# build_assistant/autogen/content_items/ContentItemCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import ContentItemConfig
from build_assistant.autogen.GenericPolymorphicCodeGenerator import GenericPolymorphicCodeGenerator
from build_assistant.autogen.content_items.constants import SUBTYPE_ENUM, BASE_CONTENT_ITEMS_CLASS
from build_assistant.autogen.content_items.common import content_item_class_name, content_item_subtype_value
from build_assistant.autogen.content_items.ContentItemFieldCodeGenerator import ContentItemFieldCodeGenerator
from build_assistant.autogen.common_code import add_deserialization_headers
from build_assistant.codegen.ParamInfo import ParamInfo


class ContentItemCodeGenerator(GenericPolymorphicCodeGenerator):
    _name = None

    def __init__(self, name, item_config, autogen_core, field_augment_override=None):
        assert isinstance(item_config, ContentItemConfig), 'Augmented object should be ContentItemConfig'

        GenericPolymorphicCodeGenerator.__init__(
            self, item_config, autogen_core, field_augment=field_augment_override or ContentItemFieldCodeGenerator
        )

        self._name = name

    def subtype_enum(self):
        return SUBTYPE_ENUM

    def subtype_value(self):
        return content_item_subtype_value(self._name)

    def mandatory_base_fields(self):
        return []

    def class_name(self):
        return content_item_class_name(self._name)

    def parent_class(self, no_template=False):
        return BASE_CONTENT_ITEMS_CLASS

    def implicitly_covered_symbols(self):
        return [
            'QDebug', 'vector', self.subtype_enum(), 'visit_subjects_callback_t'  # Through RawMessageContentItem
        ]

    def gen_key_informational_methods(self, cpp_code, _public_block, protected_block):
        self.gen_visit_subjects_methods(cpp_code, protected_block)
        protected_block.nl()

    def gen_visit_subjects_methods(self, cpp_code, protected_block):
        has_fields = any(field.is_subject_visitable() for field in self.fields)

        for alter in (False, True):
            with cpp_code.method(
                self.class_name(), ('alter' if alter else 'visit') + 'SubjectsImpl',
                'bool',
                ParamInfo(
                    type='IMM({0}_subjects_callback_t)'.format('alter' if alter else 'visit'),
                    name='callback', unused=(not has_fields)
                ),
                const=not alter, declare_in=protected_block
            ) as method:
                self.gen_visit_subjects_field_code(method, 'callback', self.fields, alter=alter)

    def gen_deserialize_methods(self, cpp_code, public_block, protected_block):
        add_deserialization_headers(cpp_code.source)

        with cpp_code.method(
            self.class_name(), 'deserializeFromStream', 'CEDE({0})'.format(self.class_name()),
            ('QDataStream&', 'mut_stream'), ParamInfo(type='bool', name='skip_type', default_value='false'),
            static=True, declare_in=public_block
        ) as method:
            method.code_line(
                'maybeDeserializeType(skip_type, mut_stream, {0}::{1})', self.subtype_enum(), self.subtype_value()
            ).nl()

            self._pre_deserialize_fields_hook(method)

            for field in self.fields:
                field.gen_deserialize_to_var(method, 'mut_stream')

            method.nl().call(
                'unique_ptr<{0}> item = make_unique<{0}>'.format(self.class_name()),
                *self._extra_deserialization_constructor_params(),
                *[field.as_rvalue() for field in self.fields if field.is_mandatory()]
            )

            for field in self.fields:
                if not field.is_mandatory():
                    method.code_line('item->{0} = {1}', field.name, field.as_rvalue())

            method.nl().ret('item')

    def _pre_deserialize_fields_hook(self, method):
        pass  # Nothing to do normally

    def _extra_deserialization_constructor_params(self):
        return []

    def gen_serialize_methods(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            'serializeToStreamImpl',
            'void',
            ParamInfo(type='QDataStream&', name='mut_stream', unused=(len(self.fields) == 0)),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            method.content_if_empty.line_comment('No fields to serialize')
            self.gen_serialize_field_code(method, 'mut_stream', self.fields)

    def gen_debug_write_methods(self, cpp_code, _public_block, protected_block):
        with cpp_code.method(
            self.class_name(),
            'writeToDebugStreamImpl',
            'void',
            ('QDebug', 'stream'),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                item_name = self.debug_write_name_override or self._name

                if len(self.fields) == 0:
                    method.code_line('stream << "[{0}]"', item_name)
                else:
                    method.code_line('stream << "[{0}"', item_name)
                    self.gen_debug_write_field_code(method, 'stream', self.fields)
                    method.code_line('stream << "]"')
