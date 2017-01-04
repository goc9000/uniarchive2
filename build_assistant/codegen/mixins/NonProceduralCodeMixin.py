# build_assistant/codegen/mixins/NonProceduralCodeMixin.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.ParamInfo import parse_param_info

import re


class NonProceduralCodeMixin:
    """
    This mixin is added to code sections that contain general non-procedural constructs, such as field, function or
    enum declarations. Note that class declarations are not included in this definition, as they can only be declared
    in a top-level code section.
    """
    def enum_class_block(self, name, underlying_type=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.declarations.EnumBlockSection import EnumBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.subsection(EnumBlockSection(self.source, name, enum_class=True, underlying_type=underlying_type))

    def declare_fn(
        self, function_name, return_type, *params, const=False, virtual=False, abstract=False, static=False
    ):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        params = _parse_params(params)

        self._use_symbols_for_params(params)
        self.source.use_symbols(return_type)

        return self._generalized_head(
            ('virtual ' if virtual else '') + ('static ' if static else '') + return_type + ' ' + function_name,
            params=[param.render_for_declaration() for param in params],
            decorations=(' const' if const else '') + (' = 0' if abstract else '')
        )

    def function(self, function_name, return_type, *params, declare_in=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.functions.FunctionBlockSection import FunctionBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        params = _parse_params(params)

        if declare_in is not None:
            declare_in.declare_fn(function_name, return_type, *params)

        self._use_symbols_for_params(params)
        self.source.use_symbols(return_type)

        return self.subsection(FunctionBlockSection(self.source, function_name, return_type, *params))

    def declare_method(self, *args, **kwargs):
        return self.declare_fn(*args, **kwargs)

    def method(
        self, class_name, function_name, return_type, *params,
        const=False, virtual=False, static=False, declare_in=None
    ):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.functions.MethodBlockSection import MethodBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        params = _parse_params(params)

        if declare_in is not None:
            declare_in.declare_method(function_name, return_type, *params, const=const, virtual=virtual, static=static)

        self._use_symbols_for_params(params)
        self.source.use_symbols(return_type)

        return self.subsection(MethodBlockSection(
            self.source,
            class_name,
            function_name,
            return_type,
            *params,
            const=const,
        ))

    def constructor(self, class_name, *params, inherits=None, declare_in=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.functions.ConstructorBlockSection import ConstructorBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        params = _parse_params(params)

        if declare_in is not None:
            declare_in.declare_constructor(class_name, *params)

        self._use_symbols_for_params(params)

        return self.subsection(ConstructorBlockSection(self.source, class_name, *params, inherits=inherits))

    def field(self, type, name, default_value=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbol(type)

        return self.line('{0} {1}{2};'.format(type, name, ' = ' + default_value if default_value is not None else ''))

    def _use_symbols_for_params(self, params):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbols(*(param.type for param in params))


def _parse_params(raw_params):
    return [parse_param_info(raw_param) for raw_param in raw_params]
