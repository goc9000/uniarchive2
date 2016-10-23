# build_assistant/codegen/mixins/NonProceduralCodeMixin.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re


class NonProceduralCodeMixin:
    """
    This mixin is added to code sections that contain general non-procedural constructs, such as field, function or
    enum declarations. Note that class declarations are not included in this definition, as they can only be declared
    in a top-level code section.
    """
    def enum_class_block(self, name):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.EnumBlockSection import EnumBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.subsection(EnumBlockSection(self.source, name, enum_class=True))

    def declare_fn(self, function_name, return_type, *params, const=False, virtual=False, abstract=False):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self._generalized_head(
            ('virtual ' if virtual else '') + return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params],
            decorations=(' const' if const else '') + (' = 0' if abstract else '')
        )

    def function(self, function_name, return_type, *params, declare_in=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.functions.FunctionBlockSection import FunctionBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        if declare_in is not None:
            declare_in.declare_fn(function_name, return_type, *_adjust_params_for_declare(params))

        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self.subsection(FunctionBlockSection(self.source, function_name, return_type, *params))

    def declare_method(self, *args, **kwargs):
        return self.declare_fn(*args, **kwargs)

    def method(self, class_name, function_name, return_type, *params, const=False, virtual=False, declare_in=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.functions.MethodBlockSection import MethodBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        if declare_in is not None:
            declare_in.declare_method(
                function_name, return_type, *_adjust_params_for_declare(params), const=const, virtual=virtual
            )

        self.source.use_symbols(return_type, *(type for type, _ in params))

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

        if declare_in is not None:
            declare_in.declare_constructor(class_name, *_adjust_params_for_declare(params))

        self.source.use_symbols(*(type for type, _ in params))

        return self.subsection(ConstructorBlockSection(self.source, class_name, *params, inherits=inherits))

    def field(self, type, name, default_value=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbol(type)

        return self.line('{0} {1}{2};'.format(type, name, ' = ' + default_value if default_value is not None else ''))


def _adjust_params_for_declare(params):
    return [(re.sub(r'\s+UNUSED\s*$', '', type), name) for type, name in params]
