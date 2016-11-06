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

    def gen_code_impl(self, cpp_source, h_source, public_block, protected_block, private_block):
        with public_block as block:
            self.gen_field_declarations(block)
            self.gen_constructors(cpp_source.code, block)
            self.gen_mandatory_fields_sanity_check_method(cpp_source.code, private_block)
            block.nl()
            self.gen_subtype_method(cpp_source.code, block)

        self.gen_protected_block_code(cpp_source.code, protected_block)

    def gen_protected_block_code(self, cpp_code, struct_block):
        self._gen_debug_write_method(cpp_code, struct_block)

    def implicitly_covered_symbols(self):
        return [
            'QDebug', 'vector', self.subtype_enum()  # Through RawMessageContentItem
        ]

    def _gen_debug_write_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(),
            'writeToDebugStreamImpl',
            'void',
            ('QDebug', 'stream'),
            const=True, virtual=True, declare_in=struct_block
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
