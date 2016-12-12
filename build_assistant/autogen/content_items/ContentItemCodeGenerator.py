# build_assistant/autogen/content_items/ContentItemCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import ContentItemConfig
from build_assistant.autogen.GenericPolymorphicCodeGenerator import GenericPolymorphicCodeGenerator
from build_assistant.autogen.content_items.constants import SUBTYPE_ENUM
from build_assistant.autogen.content_items.ContentItemFieldCodeGenerator import ContentItemFieldCodeGenerator
from build_assistant.util.grammar import camelcase_to_underscore


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
        return camelcase_to_underscore(self._name).upper()

    def mandatory_base_fields(self):
        return []

    def class_name(self):
        return self._name

    def parent_class(self, no_template=False):
        return 'RawMessageContentItem'

    def implicitly_covered_symbols(self):
        return [
            'QDebug', 'vector', self.subtype_enum()  # Through RawMessageContentItem
        ]

    def gen_serialize_methods(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            'serializeToStreamImpl',
            'void',
            ('QDataStream&' + (' UNUSED' if len(self.fields) == 0 else ''), 'mut_stream'),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            self.gen_serialize_field_code(method, self.fields)

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
                    self.gen_debug_write_field_code(method, self.fields)
                    method.code_line('stream << "]"')
