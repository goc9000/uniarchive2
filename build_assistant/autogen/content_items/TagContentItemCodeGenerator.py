# build_assistant/autogen/content_items/TagContentItemCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import TagContentItemType, TagContentItemConfig
from build_assistant.autogen.ConstructorInfo import ConstructorInfo
from build_assistant.autogen.content_items.ContentItemCodeGenerator import ContentItemCodeGenerator
from build_assistant.autogen.content_items.TagContentItemFieldCodeGenerator import TagContentItemFieldCodeGenerator
from build_assistant.codegen.codegen_utils import cpp_string_literal


class TagContentItemCodeGenerator(ContentItemCodeGenerator):
    def __init__(self, name, item_config, autogen_core):
        assert isinstance(item_config, TagContentItemConfig), 'Augmented object should be ContentItemTagConfig'

        ContentItemCodeGenerator.__init__(
            self, name, item_config, autogen_core,
            field_augment_override=lambda field_config, core: TagContentItemFieldCodeGenerator(field_config, core, self)
        )

    def parent_class(self, no_template=False):
        if self.tag_type == TagContentItemType.STANDARD:
            return 'StandardTag'
        elif self.tag_type == TagContentItemType.SYMMETRIC:
            return 'SymmetricTag'
        elif self.tag_type == TagContentItemType.SELF_CLOSING:
            return 'SelfClosingTag'

        assert False, 'Unsupported tag_type: {0}'.format(self.tag_type)

    def constructors(self):
        if self.tag_type == TagContentItemType.SYMMETRIC:
            for constructor in super().constructors():
                yield constructor._replace(
                    params=[('bool', 'open')] + constructor.params,
                    subconstructors=['SymmetricTag(open)'] + constructor.subconstructors[1:]
                )
        elif self.tag_type == TagContentItemType.STANDARD:
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

    def implicitly_covered_symbols(self):
        return super().implicitly_covered_symbols() + [
            'QString',  # Through AbstractTag
        ]

    def gen_key_informational_methods(self, cpp_code, public_block):
        with cpp_code.method(
            self.class_name(), 'tagName', 'QString', const=True, virtual=True, declare_in=public_block
        ) as method:
            method.ret('{0}', cpp_string_literal(self._tag_name_for_display()))

    def _debug_write_method_name(self):
        return 'write' + ('OpenTag' if self.tag_type == TagContentItemType.STANDARD else '') + 'AttributesToDebugStream'

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
