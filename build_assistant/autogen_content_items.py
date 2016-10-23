# build_assistant/autogen_content_items.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.AutoGenConfig import ContentItemConfig, ContentItemFieldConfig, ContentItemTagType, \
    ContentItemTagConfig, ContentItemTagFieldConfig, EnumConfig, EnumValue
from build_assistant.codegen.codegen_utils import cpp_string_literal
from build_assistant.GenericPolymorphicAugment import GenericPolymorphicAugment, ConstructorInfo
from build_assistant.GenericPolymorphicFieldAugment import GenericPolymorphicFieldAugment
from build_assistant.util.grammar import camelcase_to_underscore
from build_assistant.util.VirtualPath import VirtualPath


BASE_CONTENT_ITEMS_PATH = VirtualPath(['intermediate_format', 'content'])
SUBTYPE_ENUM = 'RawMessageContentItemSubType'


def autogen_content_items_subtype_enum(autogen_config):
    return BASE_CONTENT_ITEMS_PATH, SUBTYPE_ENUM, EnumConfig(
        values=[
            EnumValue(
                text=name,
                constant=camelcase_to_underscore(name).upper(),
                int_value=None,
                comment=None,
            ) for _, name, _ in sorted(autogen_config.content_items, key=lambda tup: tup[0].add(tup[1]).to_text())
        ],
        internal_comment=None,
    )


def gen_content_items(autogen_config, autogen_core):
    for rel_path, name, item_config in autogen_config.content_items:
        if isinstance(item_config, ContentItemTagConfig):
            item_config = ContentItemTagConfigAugment(name, item_config, autogen_core)
        else:
            item_config = ContentItemConfigAugment(name, item_config, autogen_core)

        class_name = item_config.class_name()

        cpp_source, h_source = autogen_core.new_pair(BASE_CONTENT_ITEMS_PATH.append(rel_path), class_name)

        h_source.cover_symbols(item_config.implicitly_covered_symbols())

        with h_source.code.struct_block(class_name, inherits=[item_config.parent_class()]) as struct:
            with struct.public_block() as block:
                item_config.gen_field_declarations(block)
                item_config.gen_constructors(cpp_source.code, block)
                item_config.gen_mandatory_fields_sanity_check_method(cpp_source.code)
                block.nl()
                item_config.gen_subtype_method(cpp_source.code, block)

            struct.nl()

            with struct.protected_block() as block:
                item_config.gen_protected_block_code(cpp_source.code, block)

            item_config.gen_private_block(struct)


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

    def gen_subtype_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(), 'subType', SUBTYPE_ENUM, const=True, virtual=True, declare_in=struct_block
        ) as method:
            method.code_line("return {0}::{1}", SUBTYPE_ENUM, camelcase_to_underscore(self._name).upper())

    def gen_protected_block_code(self, cpp_code, struct_block):
        self._gen_debug_write_method(cpp_code, struct_block)

    def implicitly_covered_symbols(self):
        return [
            'QDebug', 'vector', SUBTYPE_ENUM  # Through RawMessageContentItem
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

    def constructors(self):
        if self.tag_type == ContentItemTagType.SYMMETRIC:
            for constructor in super().constructors():
                yield constructor._replace(
                    params=[('bool', 'open')] + constructor.params,
                    subconstructors=['SymmetricTag(open)'] + constructor.subconstructors[1:]
                )
        elif self.tag_type == ContentItemTagType.STANDARD:
            closed_constructor_covered = False

            for constructor in super().constructors():
                if len(constructor.params) == 0:
                    yield constructor._replace(
                        params=[('bool', 'open')],
                        subconstructors=['StandardTag(open)'] + constructor.subconstructors[1:]
                    )
                    closed_constructor_covered = True
                else:
                    yield constructor._replace(
                        subconstructors=['StandardTag(true)'] + constructor.subconstructors[1:]
                    )

            if not closed_constructor_covered:
                yield ConstructorInfo(
                    params=[],
                    subconstructors=['StandardTag(false)'],
                    init_statements=[],
                    extra_fields=[]
                )

        else:
            for constructor in super().constructors():
                yield constructor

    def gen_protected_block_code(self, cpp_code, struct_block):
        self._gen_tag_name_method(cpp_code, struct_block)
        self._gen_debug_write_method(cpp_code, struct_block)

    def implicitly_covered_symbols(self):
        return super().implicitly_covered_symbols() + [
            'QString',  # Through AbstractTag
        ]

    def _gen_tag_name_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(), 'tagName', 'QString', const=True, virtual=True, declare_in=struct_block
        ) as method:
            method.code_line("return {0}", cpp_string_literal(self._tag_name_for_display()))

    def _debug_write_method_name(self):
        return 'write' + ('OpenTag' if self.tag_type == ContentItemTagType.STANDARD else '') + 'AttributesToDebugStream'

    def _gen_debug_write_method(self, cpp_code, struct_block):
        if len(self.fields) == 0 and not self.custom_debug_write_method:
            return

        with cpp_code.method(
            self.class_name(),
            self._debug_write_method_name(),
            'void',
            ('QDebug', 'stream'),
            const=True, virtual=True, declare_in=struct_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                self.gen_debug_write_field_code(method, self.fields)

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

    def _extra_clone_params(self):
        return dict(parent_tag = self._parent_tag)

    def _is_optional_for_storage(self):
        return self.is_optional or self._parent_tag.tag_type == ContentItemTagType.STANDARD

    def debug_write_header(self):
        if self.is_primary:
            return '" "'

        return super().debug_write_header()
