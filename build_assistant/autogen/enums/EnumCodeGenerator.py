# build_assistant/autogen/enums/EnumCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import EnumConfig
from build_assistant.codegen.codegen_utils import cpp_string_literal
from build_assistant.util.Augment import Augment
from build_assistant.util.grammar import camelcase_to_underscore, classname_to_varname


class EnumCodeGenerator(Augment):
    _name = None
    _core = None

    def __init__(self, name, enum_config, autogen_core):
        Augment.__init__(self, enum_config, EnumConfig)

        self._name = name
        self._core = autogen_core

    def gen_code(self, cpp_source, h_source):
        if self.internal_comment is not None:
            h_source.code.line_comment(self.internal_comment).nl()

        with h_source.code.enum_class_block(self._name) as block:
            for value in self.values:
                self._gen_value_declaration(block, value)

        self._gen_name_for_function(cpp_source, h_source)
        h_source.code.nl()
        self._gen_debug_write_operator(cpp_source, h_source)
        self._gen_serialize_operator(cpp_source, h_source)

    def as_var_name(self):
        return classname_to_varname(self._name)

    def as_parameter_spec(self):
        return self._name, self.as_var_name()

    def _name_for_function(self):
        return 'name_for_' + camelcase_to_underscore(self._name)

    def _gen_value_declaration(self, enum_block, value):
        line = value.constant

        if value.int_value is not None:
            line += ' = {0}'.format(value.int_value)

        line += ','

        if value.comment is not None:
            line += ' // ' + value.comment

        enum_block.line(line)

    def _gen_name_for_function(self, cpp_source, h_source):
        with cpp_source.code.function(
            self._name_for_function(), 'QString', self.as_parameter_spec(), declare_in=h_source.code
        ) as fn:
            with fn.switch_block(self.as_var_name()) as sw:
                for value in self.values:
                    sw.case_block(self._name + '::' + value.constant).ret('{0}', cpp_string_literal(value.text))

            fn.code_line('invariant_violation("Invalid {0} value (%d)", {1})', self._name, self.as_var_name())

    def _gen_debug_write_operator(self, cpp_source, h_source):
        cpp_source.qt_include('QtDebug').use_symbol('QP').code.function(
            'operator<< ',
            'QDebug',
            ('QDebug', 'stream'),
            self.as_parameter_spec(),
            declare_in=h_source.code
        ).code_line('stream << QP({0}({1}))', self._name_for_function(), self.as_var_name()) \
            .nl() \
            .ret('stream')

    def _gen_serialize_operator(self, cpp_source, h_source):
        cpp_source.code.function(
            'operator<< ',
            'QDataStream&',
            ('QDataStream&', 'mut_stream'),
            self.as_parameter_spec(),
            declare_in=h_source.code
        ).code_line('mut_stream << (uint32_t){0}', self.as_var_name()) \
            .nl() \
            .ret('mut_stream')
