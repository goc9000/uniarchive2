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
from build_assistant.AutoGeneratedSource import CodeGenConfig
from build_assistant.AutoGenConfig import parse_autogen_config
from build_assistant.AutoGenCore import AutoGenCore
from build_assistant.SymbolRegistry import SymbolRegistry, TypeKind
from build_assistant.autogen_common import get_full_autogen_raw_event_path_and_name
from build_assistant.util import fail, scan_files, singular


AppMetadata = namedtuple('AppMetadata', ['app_name', 'copyright_text', 'license_text'])
ConstructorInfo = namedtuple('ConstructorInfo', ['params', 'subconstructors', 'init_statements'])


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


def gen_enums(enums_config, autogen_core):
    for path, name, enum_config in enums_config:
        varname = classname_to_varname(name)
        parameter_spec = (name, varname)

        name_for_function = 'name_for_' + camelcase_to_underscore(name)

        cpp_source, h_source = autogen_core.new_pair(path, name)

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

        h_source \
            .declare_fn(name_for_function, 'QString', parameter_spec).nl() \
            .declare_fn('operator<< ', 'QDebug', ('QDebug', 'stream'), parameter_spec)

        cpp_source \
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


def gen_raw_events(autogen_config, autogen_core):
    def local_name(field_config):
        return field_config.short_name or camelcase_to_underscore(field_config.name)

    def as_field_decl(field_config):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
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

        return cpp_type, field_config.name, field_config.default_value

    def as_param(field_config):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
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

        return cpp_type, local_name(field_config)

    def as_rvalue(field_config):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        rvalue = local_name(field_config)

        if type_info.type_kind == TypeKind.POLYMORPHIC or type_info.type_kind == TypeKind.MOVABLE:
            rvalue = 'move({0})'.format(rvalue)

        return rvalue

    def as_subconstructor(field_config):
        return '{0}({1})'.format(field_config.name, as_rvalue(field_config))

    def is_mandatory_field(field_config):
        return not field_config.is_optional and field_config.default_value is None

    def constructors(event_config):
        if event_config is not None:
            base_fields = list(filter(is_mandatory_field, autogen_config.base_raw_event.fields))
            free_fields = event_config.fields

            parent_class = 'RawEvent' if not is_failable else 'RawFailableEvent'
            parent_constructor = [parent_class + '(' + ', '.join(map(as_rvalue, base_fields)) + ')']
        else:
            base_fields = []
            free_fields = autogen_config.base_raw_event.fields
            parent_constructor = []

        maybe_addable_fields = filter(lambda f: f.add_to_constructor, free_fields)
        extra_enabled_fields = set()

        while True:
            inited_fields = [f for f in free_fields if f.name in extra_enabled_fields or is_mandatory_field(f)]

            yield ConstructorInfo(
                params=[as_param(f) for f in base_fields + inited_fields],
                subconstructors=parent_constructor + [as_subconstructor(f) for f in inited_fields],
                init_statements=list(),
            )

            # Generate convenience constructor for the first singularizable field
            for index, field_config in enumerate(inited_fields):
                if field_config.maybe_singleton:
                    singularized = field_config._replace(
                        is_list=False,
                        name=singular(field_config.name),
                        short_name=singular(field_config.short_name) if field_config.short_name is not None else None
                    )

                    params = \
                        [as_param(f) for f in base_fields + inited_fields[:index]] + \
                        [as_param(singularized)] + \
                        [as_param(f) for f in inited_fields[index+1:]]

                    subcons = parent_constructor + \
                        [as_subconstructor(f) for f in inited_fields[:index]] + \
                        [as_subconstructor(f) for f in inited_fields[index+1:]]

                    yield ConstructorInfo(
                        params=params,
                        subconstructors=subcons,
                        init_statements=[
                            '{0}.push_back({1});'.format(field_config.name, as_rvalue(singularized))
                        ]
                    )
                    break

            field_config = next(maybe_addable_fields, None)
            if field_config is None:
                break

            extra_enabled_fields.add(field_config.name)

    def gen_debug_write_method(cpp_source, event_config):
        def as_rvalue_expr(field_config):
            base_type = field_config.base_type
            type_info = autogen_core.symbol_registry.lookup(base_type)
            assert type_info.is_type, '{0} is not a base type'.format(base_type)

            rvalue_expr = field_config.name

            if type_info.type_kind == TypeKind.POLYMORPHIC and not field_config.is_list:
                rvalue_expr += '.get()'
            else:
                if field_config.is_optional:
                    rvalue_expr = '*' + rvalue_expr
                if field_config.is_list:
                    cpp_source.include("utils/qt/debug_extras.h")  # For printing vectors

            return rvalue_expr

        def commit_regular_fields(block, regular_fields_line):
            if regular_fields_line is not None:
                block.line(regular_fields_line + ';')

        def write_regular_field(block, regular_fields_line, field_config):
            if regular_fields_line is None:
                regular_fields_line = 'stream'

            added_text = ' << " {0}=" << {1}'.format(local_name(field_config), as_rvalue_expr(field_config))

            if not block.line_fits(regular_fields_line + added_text + ';'):
                commit_regular_fields(block, regular_fields_line)
                regular_fields_line = 'stream'

            regular_fields_line += added_text

            return regular_fields_line

        def write_irregular_field(block, field_config):
            if field_config.is_optional:
                with block.if_block(field_config.name, nl_after=False) as b:
                    write_irregular_field2(b, field_config)
            else:
                write_irregular_field2(block, field_config)

        def write_irregular_field2(block, field_config):
            if field_config.maybe_singleton:
                name = local_name(field_config)
                rvalue = as_rvalue_expr(field_config)

                with block.if_block('{0}.size() == 1'.format(field_config.name), nl_after=False) as b:
                    b.line('stream << " {0}=" << {1}.front();'.format(singular(name), rvalue))
                    with b.else_block() as e:
                        b.line('stream << " {0}=" << {1};'.format(name, rvalue))
            else:
                write_irregular_field3(block, field_config)

        def write_irregular_field3(block, field_config):
            block.line('stream << " {0}=" << {1};'.format(local_name(field_config), as_rvalue_expr(field_config)))

        stream_type = 'QDebug' + (' UNUSED' if len(event_config.fields) == 0 else '')

        regular_fields_line = None

        with cpp_source.method(class_name, debug_write_method, 'void', (stream_type, 'stream'), const=True) as method:
            for field_config in event_config.fields:

                # First, write regular fields
                if not (field_config.is_optional or field_config.maybe_singleton):
                    regular_fields_line = write_regular_field(method, regular_fields_line, field_config)
                    continue

                commit_regular_fields(method, regular_fields_line)
                regular_fields_line = None

                write_irregular_field(method, field_config)

            commit_regular_fields(method, regular_fields_line)

    def gen_base_raw_event():
        base_path = VirtualPath(['intermediate_format', 'events'])
        class_name = 'RawEvent'

        cpp_source, h_source = autogen_core.new_pair(base_path, class_name)

        with h_source.struct_block(class_name) as struct:
            with struct.public_block() as block:
                if len(autogen_config.base_raw_event.fields) > 0:
                    for index, field in enumerate(autogen_config.base_raw_event.fields):
                        if index in autogen_config.base_raw_event.field_breaks:
                            block.nl()

                        block.field(*as_field_decl(field))

                    block.nl()

                for ctor_info in constructors(None):
                    block.declare_constructor(class_name, *ctor_info.params)

                block \
                    .nl().line('POLYMORPHIC_HELPERS').include("utils/language/polymorphic_helpers.h").nl() \
                    .declare_fn('eventName', 'QString', const=True, virtual=True, abstract=True) \
                    .declare_fn('writeToDebugStream', 'void', ('QDebug', 'stream'), const=True)

            struct.nl()

            with struct.protected_block() as block:
                block.declare_fn(
                    'writeDetailsToDebugStream',
                    'void', ('QDebug', 'stream'),
                    virtual=True,
                    const=True
                )

        h_source.declare_fn('operator<< ', 'QDebug', ('QDebug', 'stream'), ('CPTR(RawEvent)', 'event'))

        cpp_source.cover_symbols(h_source.get_covered_symbols())

        return h_source

    base_event_h = gen_base_raw_event()

    for path, name, event_config in autogen_config.raw_events:
        path, class_name = get_full_autogen_raw_event_path_and_name(path, name)

        is_failable = event_config.fail_reason_enum is not None

        cpp_source, h_source = autogen_core.new_pair(path, class_name)

        parent_class = 'RawEvent' if not is_failable else 'RawFailableEvent<' + event_config.fail_reason_enum + '>'
        debug_write_method = 'write' + ('FailableEvent' if is_failable else '') + 'DetailsToDebugStream'

        with h_source.struct_block(class_name, inherits=[parent_class]) as struct:
            h_source.cover_symbols(base_event_h.get_covered_symbols())

            with struct.public_block() as block:
                if len(event_config.fields) > 0:
                    for index, field in enumerate(event_config.fields):
                        if index in event_config.field_breaks:
                            block.nl()

                        block.field(*as_field_decl(field))

                    block.nl()

                for ctor_info in constructors(event_config):
                    block.declare_constructor(class_name, *ctor_info.params)

                block.nl().declare_fn('eventName', 'QString', const=True, virtual=True)

            struct.nl()

            with struct.protected_block() as block:
                block.declare_fn(debug_write_method, 'void', ('QDebug', 'stream'), virtual=True, const=True)

        cpp_source.cover_symbols(h_source.get_covered_symbols())

        for ctor_info in constructors(event_config):
            with cpp_source.constructor(class_name, *ctor_info.params, inherits=ctor_info.subconstructors) as cons:
                for line in ctor_info.init_statements:
                    cons.line(line)

        with cpp_source.method(class_name, 'eventName', 'QString', const=True) as m:
            m.line("return {0};".format(m.string_literal(name)))

        gen_debug_write_method(cpp_source, event_config)


def autogenerate_code():
    with open('autogen_config.yml', 'r') as f:
        raw_config = yaml.load(f)

    autogen_config = parse_autogen_config(raw_config)
    symbol_registry = SymbolRegistry.build(BASE_SRC_DIR, autogen_config, CODE_GEN_CFG)

    autogen_core = AutoGenCore(autogen_config, symbol_registry, CODE_GEN_CFG)

    gen_enums(autogen_config.enums, autogen_core)
    gen_raw_events(autogen_config, autogen_core)

    autogen_core.commit(BASE_SRC_DIR)


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
