# build_assistant/autogen/GenericPolymorphicFieldCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import GenericPolymorphicFieldConfig
from build_assistant.codegen.codegen_utils import cpp_string_literal
from build_assistant.codegen.ParamInfo import ParamInfo
from build_assistant.autogen.SymbolRegistry import TypeKind
from build_assistant.util.Augment import Augment
from build_assistant.util.grammar import camelcase_to_underscore, singular


class GenericPolymorphicFieldCodeGenerator(Augment):
    _core = None

    _base_type = None
    _type_info = None

    def __init__(self, field_config, autogen_core):
        Augment.__init__(self, field_config, GenericPolymorphicFieldConfig)

        self._core = autogen_core

        self._base_type = field_config.base_type
        self._type_info = autogen_core.symbol_registry.lookup(self._base_type)
        assert self._type_info.is_type, '{0} is not a base type'.format(self._base_type)

    def _extra_clone_params(self):
        return dict()

    def local_name(self):
        return self.short_name or camelcase_to_underscore(self.name)

    def as_field_decl(self):
        cpp_type = self._base_type
        if self.uses_unique_ptr():
            cpp_type = 'unique_ptr<{0}>'.format(cpp_type)
        if self.is_list:
            cpp_type = 'vector<{0}>'.format(cpp_type)
        if self.uses_optional():
            cpp_type = 'optional<{0}>'.format(cpp_type)

        return cpp_type, self.name, self.default_value

    def uses_optional(self):
        """Whether storage for this field uses the optional<T> template"""
        return self._is_optional_for_storage() and not \
            (self._type_info.type_kind == TypeKind.POLYMORPHIC and not self.is_list)

    def _is_optional_for_storage(self):
        """
        Whether this field should be optional as far as storage is concerned, regardless of whether it is optional
        semantics-wise. It is possible for a field to be semantically mandatory, but still optional in storage (e.g.
        if a StandardTag is closed, it stores no attributes, but in the open variant, it can have mandatory attributes.
        """
        return self.is_optional

    def uses_unique_ptr(self):
        """Whether storage for this field uses the unique_ptr<T> template"""
        return self._type_info.type_kind == TypeKind.POLYMORPHIC

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

        return ParamInfo(type=cpp_type, name=self.local_name())

    def as_rvalue(self):
        """
        Renders a movable rvalue for a similarly-named parameter or local variable corresponding to this field.
        """
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

    def as_serialize_rvalue(self, destination_code):
        """
        Renders an expression that can be passed to a QDataStream so as to serialize this field, provided it is regular.
        """
        if self.uses_unique_ptr() or self.uses_optional() or self.is_list:
            destination_code.source.include("utils/serialization/serialization_helpers.h")

        return self.name

    def as_print_rvalue(self, destination_code):
        """
        Renders an expression for the debug-printable value of this field.
        """
        rvalue_expr = self.name

        if self._base_type == 'QUrl' and not self.is_list:
            return self.as_subfield_value('url()')

        if self.uses_unique_ptr() or self.uses_optional() or self.is_list:
            destination_code.source.include("utils/qt/debug_extras.h")

        return rvalue_expr

    def as_subfield_value(self, subfield):
        use_arrow = self.uses_optional() or (self.uses_unique_ptr() and not self.is_list)
        return self.name + ('->' if use_arrow else '.') + subfield

    def debug_write_header(self):
        """
        Renders an expression describing the header printed right before the value of the field, in the debug write
        method, e.g. " field_name="
        """
        return '" {0}="'.format(self.local_name())

    def singularized(self):
        return type(self)(
            self._augmented_object._replace(
                is_list=False,
                name=singular(self.name),
                short_name=singular(self.short_name) if self.short_name is not None else None
            ),
            self._core,
            **self._extra_clone_params(),
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
                        cpp_string_literal("Parameter '{0}' cannot have empty entries".format(self.local_name()))
                    )
            else:
                source.call(
                    'invariant',
                    value_expr,
                    cpp_string_literal("Parameter '{0}' cannot have empty value".format(self.local_name()))
                )

    def is_regular_for_serialize(self):
        return not (self.is_optional and self._type_info.type_kind == TypeKind.POLYMORPHIC and not self.is_list)

    def gen_irregular_serialize_code(self, method, stream_name):
        method.source.include("utils/serialization/serialization_helpers.h")

        method.code_line('serialize_optional_unique_ptr({0}, {1})', stream_name, self.name)

    def is_regular_for_debug_write(self):
        return not (self.is_optional or self.maybe_singleton)

    def gen_irregular_debug_write_code(self, method, stream_name):
        block = method

        if self.is_optional:
            block = block.if_block(self.name, nl_after=False)

        if self.maybe_singleton:
            if_block = block.if_block('{0} == 1'.format(self.as_subfield_value('size()')), nl_after=False) \
                .code_line(
                    '{0} << {1} << {2}',
                    stream_name,
                    self.singularized().debug_write_header(),
                    self.as_subfield_value('front()'),
                )

            block = if_block.else_block()

        block.code_line('{0} << {1} << {2}', stream_name, self.debug_write_header(), self.as_print_rvalue(block))
