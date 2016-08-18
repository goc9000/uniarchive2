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
from build_assistant.util import fail, scan_files


AppMetadata = namedtuple('AppMetadata', ['app_name', 'copyright_text', 'license_text'])

GenericEntityConfig = namedtuple('GenericEntityConfig', ['fields', 'options'])
GenericEntityFieldConfig = namedtuple('GenericEntityFieldConfig', ['expression', 'options'])

EnumConfig = namedtuple('EnumConfig', ['values', 'internal_comment'])
EnumValue = namedtuple('EnumValue', ['text', 'constant', 'int_value', 'comment'])


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


def visit_hierarchy(config, process_leaf):
    def visit_hierarchy_rec(config_view, base_path):
        for yaml_path, subconfig in config_view.items():
            path = VirtualPath.from_text(yaml_path)
            if subconfig is None:
                subconfig = dict()

            basename = path.basename()

            if basename[0].isupper():
                process_leaf(basename, base_path.append(path.parent()), subconfig)
            else:
                visit_hierarchy_rec(subconfig, base_path.append(path))

    visit_hierarchy_rec(config, VirtualPath([]))


def preparse_entity(entity_config, field_name):
    raw_fields = list()
    raw_options = dict()

    for subconfig in entity_config:
        if isinstance(subconfig, str):
            raw_fields.append(GenericEntityFieldConfig(expression=subconfig, options=dict()))
        elif field_name in subconfig:
            raw_fields.append(
                GenericEntityFieldConfig(
                    expression=subconfig[field_name],
                    options={k: v for k, v in subconfig.items() if k != field_name}
                )
            )
        else:
            raw_options.update(**subconfig)

    return GenericEntityConfig(fields=raw_fields, options=raw_options)


def gen_enums(config):
    def text_to_constant_name(text):
        return '_'.join(word.upper() for word in re.findall('[a-z0-9]+', text, flags=re.IGNORECASE))

    def parse_enum_value(preparsed_field):
        match = re.match(r'^(.*?)(?:\s+as\s+([A-Z_]+))?(?:\s*=\s*(\d+))?(?:\s*//\s*(.*))?$', preparsed_field.expression)
        assert match is not None, "Invalid enum value expression: " + preparsed_field.expression

        text, constant, int_value, comment = match.groups()

        return EnumValue(
            text=text,
            constant=constant or text_to_constant_name(text),
            int_value=int_value,
            comment=comment
        )

    def parse_enum_config(enum_config):
        preparsed = preparse_entity(enum_config, 'value')

        return EnumConfig(
            values=[parse_enum_value(field) for field in preparsed.fields],
            internal_comment=preparsed.options.get('internal comment')
        )

    def gen_enum(name, path, enum_config):
        enum_info = parse_enum_config(enum_config)

        varname = classname_to_varname(name)
        parameter_spec = name + ' ' + varname

        name_for_function = 'name_for_' + camelcase_to_underscore(name)

        h_source = AutoGeneratedSource(path, name, 'h', CODE_GEN_CFG)\
            .qt_include('QDebug')

        cpp_source = AutoGeneratedSource(path, name, 'cpp', CODE_GEN_CFG)\
            .include('utils/qt/shortcuts.h')\
            .qt_include('QtDebug')

        if enum_info.internal_comment is not None:
            h_source.line_comment(enum_info.internal_comment).nl()

        with h_source.enum_class_block(name) as block:
            for i, value in enumerate(enum_info.values):
                line = value.constant

                if value.int_value is not None:
                    line += ' = ' + value.int_value

                line += ','

                if value.comment is not None:
                    line += ' // ' + value.comment

                block.line(line)

        h_source.declare_fn(name_for_function, 'QString', parameter_spec)
        with cpp_source.function(name_for_function, 'QString', parameter_spec) as fn:
            with fn.switch_block(varname) as sw:
                for value in enum_info.values:
                    with sw.case_block(name + '::' + value.constant) as c:
                        c.line('return {0};'.format(c.string_literal(value.text)))

            fn.line('invariant_violation("Invalid {0} value (%d)", {1});'.format(name, varname))

        h_source.nl().declare_fn('operator<< ', 'QDebug', 'QDebug stream', parameter_spec)
        with cpp_source.function('operator<< ', 'QDebug', 'QDebug stream', parameter_spec) as fn:
            fn.line('stream << QP({0}({1}));'.format(name_for_function, varname))\
              .nl()\
              .line('return stream;')

        h_source.commit(BASE_SRC_DIR)
        cpp_source.commit(BASE_SRC_DIR)

    visit_hierarchy(config['enums'], gen_enum)


def gen_raw_events(config):
    def gen_raw_event(name, path, raw_event_config):
        path = VirtualPath(['intermediate_format', 'events']).append(path)
        class_name = 'Raw' + name + 'Event'

        h_source = AutoGeneratedSource(path, class_name, 'h', CODE_GEN_CFG)

        cpp_source = AutoGeneratedSource(path, class_name, 'cpp', CODE_GEN_CFG)

        h_source.commit(BASE_SRC_DIR)
        cpp_source.commit(BASE_SRC_DIR)

    visit_hierarchy(config['raw events'], gen_raw_event)


def autogenerate_code():
    with open('autogen_config.yml', 'r') as f:
        config = yaml.load(f)

    gen_enums(config)
    gen_raw_events(config)


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
