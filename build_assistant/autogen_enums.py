# build_assistant/autogen_common.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.grammar import classname_to_varname, camelcase_to_underscore


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
            .cover_symbols_from(h_source) \
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