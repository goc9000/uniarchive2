#!/usr/bin/env python3

# build_assistant.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import argparse
import os
import re
import sys
import yaml

from collections import namedtuple

from build_assistant.VirtualPath import VirtualPath
from build_assistant.AutoGeneratedSource import AutoGeneratedSource, CodeGenConfig
from build_assistant.AutoGenConfig import parse_autogen_config
from build_assistant.SymbolRegistry import SymbolRegistry, TypeKind
from build_assistant.autogen_common import get_full_autogen_raw_event_path_and_name
from build_assistant.util import fail, scan_files


AppMetadata = namedtuple('AppMetadata', ['app_name', 'copyright_text', 'license_text'])


APP_METADATA = AppMetadata(
    app_name='UniArchive II',
    copyright_text='(C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>',
    license_text='Licensed under the GPL-3',
)

CODE_GEN_CFG = CodeGenConfig(
    app_metadata=APP_METADATA,
    indent_size=4,
    gutter_width=120,
    base_namespace='uniarchive2',
)

BASE_SRC_DIR = VirtualPath.from_text('src')


def camelcase_to_underscore(name):
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)).lower()


def classname_to_varname(name):
    return camelcase_to_underscore(name).split('_')[-1]


def gen_enums(enums_config, symbol_registry):
    for path, name, enum_config in enums_config:
        varname = classname_to_varname(name)
        parameter_spec = (name, varname)

        name_for_function = 'name_for_' + camelcase_to_underscore(name)

        h_source = AutoGeneratedSource(path, name, 'h', CODE_GEN_CFG, symbol_registry)

        if enum_config.internal_comment is not None:
            h_source.line_comment(enum_config.internal_comment).nl()

        with h_source.enum_class_block(name) as block:
            for i, value in enumerate(enum_config.values):
                line = value.constant

                if value.int_value is not None:
                    line += ' = ' + value.int_value

                line += ','

                if value.comment is not None:
                    line += ' // ' + value.comment

                block.line(line)

        h_source\
            .declare_fn(name_for_function, 'QString', parameter_spec).nl()\
            .declare_fn('operator<< ', 'QDebug', ('QDebug', 'stream'), parameter_spec)\
            .commit(BASE_SRC_DIR)

        cpp_source = AutoGeneratedSource(path, name, 'cpp', CODE_GEN_CFG, symbol_registry) \
            .cover_symbols(h_source.get_covered_symbols()) \
            .include('utils/qt/shortcuts.h') \
            .qt_include('QtDebug')

        with cpp_source.function(name_for_function, 'QString', parameter_spec) as fn:
            with fn.switch_block(varname) as sw:
                for value in enum_config.values:
                    with sw.case_block(name + '::' + value.constant) as c:
                        c.line('return {0};'.format(c.string_literal(value.text)))

            fn.line('invariant_violation("Invalid {0} value (%d)", {1});'.format(name, varname))

        with cpp_source.function('operator<< ', 'QDebug', ('QDebug', 'stream'), parameter_spec) as fn:
            fn.line('stream << QP({0}({1}));'.format(name_for_function, varname)) \
              .nl() \
              .line('return stream;')

        cpp_source.commit(BASE_SRC_DIR)


def gen_raw_events(autogen_config, symbol_registry):
    def as_field_decl(field_config):
        base_type = field_config.base_type
        type_info = symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        use_optional = field_config.is_optional and not \
            (type_info.type_kind == TypeKind.POLYMORPHIC and not field_config.is_list)
        use_unique_ptr = type_info.type_kind == TypeKind.POLYMORPHIC
        use_vector = field_config.is_list

        cpp_type = base_type
        if use_unique_ptr:
            cpp_type = 'unique_ptr<{0}>'.format(cpp_type)
        if use_vector:
            cpp_type = 'vector<{0}>'.format(cpp_type)
        if use_optional:
            cpp_type = 'optional<{0}>'.format(cpp_type)

        return cpp_type, field_config.name

    def as_param(field_config):
        base_type = field_config.base_type
        type_info = symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        cpp_type = base_type

        if type_info.type_kind == TypeKind.POLYMORPHIC:
            cpp_type = ('TAKE_VEC({0})' if field_config.is_list else 'TAKE({0})').format(cpp_type)
        else:
            if field_config.is_list:
                cpp_type = 'vector<{0}>'.format(cpp_type)

            if type_info.type_kind == TypeKind.MOVABLE:
                cpp_type += '&&'
            elif type_info.type_kind != TypeKind.PRIMITIVE or field_config.is_list:
                cpp_type = 'IMM({0})'.format(cpp_type)

        return cpp_type, field_config.short_name or camelcase_to_underscore(field_config.name)

    def constructor_params(event_config):
        all_fields = \
            [field for field in autogen_config.base_raw_event.fields + event_config.fields if field is not None]

        params = list()

        for field_config in all_fields:
            if not field_config.is_optional:
                params.append(as_param(field_config))

        return params

    for path, name, event_config in autogen_config.raw_events:
        path, class_name = get_full_autogen_raw_event_path_and_name(path, name)

        is_failable = event_config.fail_reason_enum is not None

        h_source = AutoGeneratedSource(path, class_name, 'h', CODE_GEN_CFG, symbol_registry)

        parent_class = 'RawEvent' if not is_failable else 'RawFailableEvent<' + event_config.fail_reason_enum + '>'

        with h_source.struct_block(class_name, inherits=[parent_class]) as struct:
            with struct.public_block() as block:
                if len(event_config.fields) > 0:
                    for field in event_config.fields:
                        if field is None:
                            block.nl()
                        else:
                            block.field(*as_field_decl(field))

                    block.nl()

                block \
                    .declare_constructor(class_name, *constructor_params(event_config)).nl() \
                    .declare_fn('eventName', 'QString', const=True, virtual=True)

            struct.nl()

            with struct.protected_block() as block:
                block.declare_fn(
                    'write' + ('FailableEvent' if is_failable else '') + 'DetailsToDebugStream',
                    'void', ('QDebug', 'stream'),
                    virtual=True,
                    const=True,
                )

        h_source.commit(BASE_SRC_DIR)

        cpp_source = AutoGeneratedSource(path, class_name, 'cpp', CODE_GEN_CFG, symbol_registry) \
            .cover_symbols(h_source.get_covered_symbols())

        cpp_source.commit(BASE_SRC_DIR)


def autogenerate_code():
    with open('autogen_config.yml', 'r') as f:
        raw_config = yaml.load(f)

    autogen_config = parse_autogen_config(raw_config)

    symbol_registry = SymbolRegistry.build(BASE_SRC_DIR, autogen_config, CODE_GEN_CFG)

    gen_enums(autogen_config.enums, symbol_registry)
    gen_raw_events(autogen_config, symbol_registry)


def cleanup_pro_file():
    def keyfn_order_directories_first(path):
        return [' ' + element for element in path.elements[:-1]] + [path.elements[-1]]

    real_path = BASE_SRC_DIR.add('uniarchive2.pro').realize()

    with open(real_path, 'rt') as f:
        contents = f.read()

    if not contents.endswith("\n"):
        contents += "\n"

    has_effect = False

    for item_name, varname, extensions in [
        ('source', 'SOURCES', ['.cpp', '.c']),
        ('header', 'HEADERS', ['.h', '.hpp'])
    ]:
        pattern = re.compile("(?<=\n)" + varname + "\s*[+]=\s*((?:[^\n\\\\]*\\\\\n)+[^\n]*\n)")

        match = pattern.search(contents)
        assert match is not None, 'Could not find ' + varname + ' declaration in makefile'

        declared_paths = re.split("\\s+", pattern.search(contents).group(1).replace("\\\n", " ").strip())

        actual_files = scan_files(BASE_SRC_DIR, full_paths=False, filter=lambda p: p.extension().lower() in extensions)
        for file in actual_files:
            if ' ' in file.to_text():
                fail("File '{0}' contains spaces, these are not supported by the Qt infrastructure", file.realize())

        actual_paths = [f.realize() for f in sorted(actual_files, key=keyfn_order_directories_first)]

        for item in sorted(set(actual_paths) - set(declared_paths)):
            print("Added {0}: {1}".format(item_name, item))
        for item in sorted(set(declared_paths) - set(actual_paths)):
            print("Removed {0}: {1}".format(item_name, item))

        if actual_paths != declared_paths:
            has_effect = True
            print("Reordered {0}".format(varname))

        contents = pattern.sub(" \\\n    ".join([varname + " +="] + actual_paths) + "\n", contents)

    if has_effect:
        with open(real_path, 'wt') as f:
            f.write(contents)


def sanity_check():
    if not os.path.isdir(BASE_SRC_DIR.realize()):
        fail(
            "Directory '{0}' was not found. Check that you are running the script in the right location.",
            BASE_SRC_DIR.to_text()
        )


def main():
    parser = argparse.ArgumentParser(description='Build assistant for Uniarchive II')

    subparsers = parser.add_subparsers(title='commands', dest='command')

    subparsers.add_parser('gencode', help='Build autogenerated code')
    subparsers.add_parser('cleanpro', help='Cleanup source list in QT .pro file')

    raw_args = parser.parse_args(sys.argv[1:])

    if raw_args.command is None:
        parser.print_usage()
        return

    sanity_check()

    if raw_args.command == 'gencode':
        autogenerate_code()
    elif raw_args.command == 'cleanpro':
        cleanup_pro_file()

main()
