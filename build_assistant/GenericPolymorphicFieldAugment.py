# build_assistant/GenericPolymorphicFieldAugment.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.Augment import Augment
from build_assistant.AutoGenConfig import GenericPolymorphicFieldConfig
from build_assistant.grammar import camelcase_to_underscore, singular
from build_assistant.SymbolRegistry import TypeKind


class GenericPolymorphicFieldAugment(Augment):
    _core = None

    _base_type = None
    _type_info = None

    def __init__(self, field_config, autogen_core):
        Augment.__init__(self, field_config, GenericPolymorphicFieldConfig)

        self._core = autogen_core

        self._base_type = field_config.base_type
        self._type_info = autogen_core.symbol_registry.lookup(self._base_type)
        assert self._type_info.is_type, '{0} is not a base type'.format(self._base_type)

    def local_name(self):
        return self.short_name or camelcase_to_underscore(self.name)

    def as_field_decl(self):
        type_kind = self._type_info.type_kind

        use_optional = self._is_optional_for_storage() and not (type_kind == TypeKind.POLYMORPHIC and not self.is_list)
        use_unique_ptr = type_kind == TypeKind.POLYMORPHIC
        use_vector = self.is_list

        cpp_type = self._base_type
        if use_unique_ptr:
            cpp_type = 'unique_ptr<{0}>'.format(cpp_type)
        if use_vector:
            cpp_type = 'vector<{0}>'.format(cpp_type)
        if use_optional:
            cpp_type = 'optional<{0}>'.format(cpp_type)

        return cpp_type, self.name, self.default_value

    def _is_optional_for_storage(self):
        return self.is_optional

    def as_param(self):
        type_kind = self._type_info.type_kind

        cpp_type = self._base_type
        if type_kind == TypeKind.POLYMORPHIC:
            cpp_type = ('TAKE_VEC({0})' if self.is_list else 'TAKE({0})').format(cpp_type)
        else:
            if self.is_list:
                cpp_type = 'vector<{0}>'.format(cpp_type)

            if type_kind == TypeKind.MOVABLE:
                cpp_type += '&&'
            elif type_kind != TypeKind.PRIMITIVE or self.is_list:
                cpp_type = 'IMM({0})'.format(cpp_type)

        return cpp_type, self.local_name()

    def as_rvalue(self):
        type_kind = self._type_info.type_kind

        rvalue = self.local_name()

        if type_kind == TypeKind.POLYMORPHIC or type_kind == TypeKind.MOVABLE:
            rvalue = 'move({0})'.format(rvalue)

        return rvalue

    def as_initializer(self):
        """
        Renders an initializer of the field from a similarly-named parameter, e.g. fieldName(field_name)
        """
        return '{0}({1})'.format(self.name, self.as_rvalue())

    def as_print_rvalue(self, source):
        rvalue_expr = self.name

        if self._type_info.type_kind == TypeKind.POLYMORPHIC and not self.is_list:
            rvalue_expr += '.get()'
        else:
            if self.is_optional:
                rvalue_expr = '*' + rvalue_expr
            if self.is_list:
                source.include("utils/qt/debug_extras.h")  # For printing vectors

        return rvalue_expr

    def singularized(self):
        return type(self)(
            self._augmented_object._replace(
                is_list=False,
                name=singular(self.name),
                short_name=singular(self.short_name) if self.short_name is not None else None
            ),
            self._core
        )

    def is_mandatory(self):
        return not self.is_optional and self.default_value is None

    def is_checkable(self):
        type_kind = self._type_info.type_kind

        return type_kind == TypeKind.POLYMORPHIC

    def gen_param_check(self, source, disambiguate=False):
        type_kind = self._type_info.type_kind

        value_expr = 'this->{0}'.format(self.name) if disambiguate else self.name

        if type_kind == TypeKind.POLYMORPHIC:
            if self.is_list:
                with source.for_each_block('IMM(auto)', 'item', value_expr, nl_after=False) as block:
                    block.call(
                        'invariant',
                        'item',
                        source.string_literal("Parameter '{0}' cannot have empty entries".format(self.local_name()))
                    )
            else:
                source.call(
                    'invariant',
                    value_expr,
                    source.string_literal("Parameter '{0}' cannot have empty value".format(self.local_name()))
                )
