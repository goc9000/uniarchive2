# build_assistant/autogen/content_items/BaseContentItemCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import ContentItemConfig
from build_assistant.autogen.GenericPolymorphicCodeGenerator import GenericPolymorphicCodeGenerator
from build_assistant.autogen.content_items.constants import SUBTYPE_ENUM, BASE_CONTENT_ITEMS_CLASS
from build_assistant.autogen.content_items.ContentItemFieldCodeGenerator import ContentItemFieldCodeGenerator


class BaseContentItemCodeGenerator(GenericPolymorphicCodeGenerator):
    def __init__(self, autogen_core):
        pseudo_item_config = ContentItemConfig(
            fields=[],
            field_breaks=[],
            custom_debug_write_method=False,
            debug_write_name_override=None,
        )

        GenericPolymorphicCodeGenerator.__init__(
            self, pseudo_item_config, autogen_core, field_augment=ContentItemFieldCodeGenerator
        )

    def subtype_enum(self):
        return SUBTYPE_ENUM

    def mandatory_base_fields(self):
        return list()

    def class_name(self):
        return BASE_CONTENT_ITEMS_CLASS

    def parent_class(self, no_template=None):
        return 'StandardPolymorphic' if no_template else 'StandardPolymorphic<{0}>'.format(self.subtype_enum())

    def interfaces(self):
        return ['IApparentSubjectVisitable']

    def gen_subtype_method(self, _cpp_code, _public_block):
        pass  # Do not generate this method as this is a base class

    def gen_key_informational_methods(self, cpp_code, public_block, protected_block):
        self.gen_visit_subjects_methods(cpp_code, public_block, protected_block)
        public_block.nl()
        protected_block.nl()

    def gen_visit_subjects_methods(self, cpp_code, public_block, protected_block):
        with cpp_code.method(
            self.class_name(), 'visitSubjects', 'bool', ('IApparentSubjectVisitor&', 'visitor'), declare_in=public_block
        ) as method:
            self.gen_visit_subjects_field_code(method, 'visitor', self.fields, 'visitSubjectsImpl(visitor)')

        protected_block.declare_method(
            'visitSubjectsImpl', 'bool', ('IApparentSubjectVisitor&', 'visitor'),
            virtual=True, abstract=True
        )

    def gen_deserialize_methods(self, cpp_code, public_block, protected_block):
        from build_assistant.autogen.content_items.gen_main import autogen_content_items_index

        item_index = autogen_content_items_index(cpp_code.source.core.autogen_config, include_base=False)

        self.gen_deserialization_manifold(cpp_code, public_block, item_index)

    def gen_serialize_methods(self, cpp_code, protected_block):
        protected_block.declare_method(
            'serializeToStreamImpl', 'void', ('QDataStream&', 'mut_stream'),
            const=True, virtual=True, abstract=True
        )

    def gen_debug_write_methods(self, cpp_code, public_block, protected_block):
        with cpp_code.method(
            self.class_name(), 'writeToDebugStream', 'void', ('QDebug', 'stream'), const=True, declare_in=public_block
        ) as method:
            method \
                .declare_var('QDebugStateSaver', 'saver(stream)') \
                .code_line('stream.nospace()').nl() \
                .code_line('writeToDebugStreamImpl(stream)').nl()

        protected_block.declare_method(
            'writeToDebugStreamImpl', 'void', ('QDebug', 'stream'),
            const=True, virtual=True, abstract=True
        )
