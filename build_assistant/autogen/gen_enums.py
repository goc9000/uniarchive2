# build_assistant/autogen/gen_enums.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.gen_main import autogen_raw_events_subtype_enum
from build_assistant.autogen.content_items.gen_main import autogen_content_items_subtype_enum
from build_assistant.codegen.codegen_utils import cpp_string_literal
from build_assistant.util.grammar import classname_to_varname, camelcase_to_underscore


def autogen_enums_index(autogen_config):
    for path, name, _ in all_enums(autogen_config):
        yield path, name


def all_enums(autogen_config):
    for item in autogen_config.enums:
        yield item
    for item in synthetic_enums(autogen_config):
        yield item


def synthetic_enums(autogen_config):
    yield autogen_raw_events_subtype_enum(autogen_config)
    yield autogen_content_items_subtype_enum(autogen_config)


def gen_enums(autogen_config, autogen_core):
    for path, name, enum_config in all_enums(autogen_config):
        cpp_source, h_source = autogen_core.new_pair(path, name)

        if enum_config.internal_comment is not None:
            h_source.code.line_comment(enum_config.internal_comment).nl()

        with h_source.code.enum_class_block(name) as block:
            for i, value in enumerate(enum_config.values):
                line = value.constant

                if value.int_value is not None:
                    line += ' = ' + value.int_value

                line += ','

                if value.comment is not None:
                    line += ' // ' + value.comment

                block.line(line)

        varname = classname_to_varname(name)
        parameter_spec = (name, varname)
        name_for_function = 'name_for_' + camelcase_to_underscore(name)

        with cpp_source.code.function(name_for_function, 'QString', parameter_spec, declare_in=h_source.code) as fn:
            with fn.switch_block(varname) as sw:
                for value in enum_config.values:
                    sw.case_block(name + '::' + value.constant).ret('{0}', cpp_string_literal(value.text))

            fn.code_line('invariant_violation("Invalid {0} value (%d)", {1})', name, varname)

        h_source.code.nl()

        cpp_source.qt_include('QtDebug').use_symbol('QP').code.function(
            'operator<< ', 'QDebug', ('QDebug', 'stream'), parameter_spec, declare_in=h_source.code
        ).code_line('stream << QP({0}({1}))', name_for_function, varname) \
            .nl() \
            .ret('stream')

        cpp_source.code.function(
            'operator<< ', 'QDataStream&', ('QDataStream&', 'mut_stream'), parameter_spec, declare_in=h_source.code
        ).code_line('mut_stream << (uint32_t){0}', varname) \
            .nl() \
            .ret('mut_stream')
