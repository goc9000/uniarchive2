# build_assistant/autogen_content_items.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.VirtualPath import VirtualPath
from build_assistant.AutoGenConfig import ContentItemConfig, ContentItemFieldConfig, ContentItemTagType, \
    ContentItemTagConfig, ContentItemTagFieldConfig
from build_assistant.GenericPolymorphicAugment import GenericPolymorphicAugment
from build_assistant.GenericPolymorphicFieldAugment import GenericPolymorphicFieldAugment


BASE_CONTENT_ITEMS_PATH = VirtualPath(['intermediate_format', 'content'])


def gen_content_items(autogen_config, autogen_core):
    for rel_path, name, item_config in autogen_config.content_items:
        if isinstance(item_config, ContentItemTagConfig):
            item_config = ContentItemTagConfigAugment(name, item_config, autogen_core)
        else:
            item_config = ContentItemConfigAugment(name, item_config, autogen_core)

        class_name = item_config.class_name()

        cpp_source, h_source = autogen_core.new_pair(BASE_CONTENT_ITEMS_PATH.append(rel_path), class_name)

        with h_source.struct_block(class_name, inherits=[item_config.parent_class()]) as struct:
            with struct.public_block() as block:
                item_config.gen_field_declarations(block)

            struct.nl()

            with struct.protected_block() as _:
                item_config.gen_protected_block_code(cpp_source)


class ContentItemConfigAugment(GenericPolymorphicAugment):
    _name = None

    def __init__(self, name, item_config, autogen_core, field_augment_override=None):
        assert isinstance(item_config, ContentItemConfig), 'Augmented object should be ContentItemConfig'

        GenericPolymorphicAugment.__init__(
            self, item_config, autogen_core, field_augment=field_augment_override or ContentItemFieldAugment
        )

        self._name = name

    def mandatory_base_fields(self):
        return []

    def class_name(self):
        return self._name

    def parent_class(self, no_template=False):
        return 'RawMessageContentItem'

    def gen_protected_block_code(self, block):
        pass


class ContentItemFieldAugment(GenericPolymorphicFieldAugment):
    def __init__(self, field_config, autogen_core):
        assert isinstance(field_config, ContentItemFieldConfig), 'Augmented object should be ContentItemFieldConfig'

        GenericPolymorphicFieldAugment.__init__(self, field_config, autogen_core)


class ContentItemTagConfigAugment(ContentItemConfigAugment):
    def __init__(self, name, item_config, autogen_core):
        assert isinstance(item_config, ContentItemTagConfig), 'Augmented object should be ContentItemTagConfig'

        ContentItemConfigAugment.__init__(
            self, name, item_config, autogen_core,
            field_augment_override=lambda field_config, core: ContentItemTagFieldAugment(field_config, core, self)
        )

    def parent_class(self, no_template=False):
        if self.tag_type == ContentItemTagType.STANDARD:
            return 'StandardTag'
        elif self.tag_type == ContentItemTagType.SYMMETRIC:
            return 'SymmetricTag'
        elif self.tag_type == ContentItemTagType.SELF_CLOSING:
            return 'SelfClosingTag'

        assert False, 'Unsupported tag_type: {0}'.format(self.tag_type)

    def gen_protected_block_code(self, cpp_source):
        self._gen_tag_name_method(cpp_source)

    def _gen_tag_name_method(self, cpp_source):
        with cpp_source.method(
            self.class_name(), 'tagName', 'QString', const=True, virtual=True, declare=True
        ) as method:
            method.line("return {0};".format(method.string_literal(self._tag_name_for_display())))

    def _tag_name_for_display(self):
        if self.tag_name_override is not None:
            return self.tag_name_override

        return self._name[:-3] if self._name.endswith('Tag') else self._name


class ContentItemTagFieldAugment(ContentItemFieldAugment):
    _parent_tag = None

    def __init__(self, field_config, autogen_core, parent_tag):
        assert isinstance(field_config, ContentItemTagFieldConfig), \
            'Augmented object should be ContentItemTagFieldConfig'

        ContentItemFieldAugment.__init__(self, field_config, autogen_core)

        self._parent_tag = parent_tag

    def _is_optional_for_storage(self):
        return self.is_optional or self._parent_tag.tag_type == ContentItemTagType.STANDARD
