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
from build_assistant.codegen.ParamInfo import ParamInfo


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

    def constructors(self, *args, **kwargs):
        if self.tag_type == TagContentItemType.SYMMETRIC:
            for constructor in super().constructors(*args, **kwargs):
                yield constructor._replace(
                    params=[ParamInfo(type='bool', name='open')] + constructor.params,
                    subconstructors=['SymmetricTag(open)'] + constructor.subconstructors[1:]
                )
        elif self.tag_type == TagContentItemType.STANDARD:
            closed_constructor_covered = False

            for constructor in super().constructors(*args, **kwargs):
                if len(constructor.params) == 0:
                    yield constructor._replace(
                        params=[ParamInfo(type='bool', name='open')],
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
            for constructor in super().constructors(*args, **kwargs):
                yield constructor

    def implicitly_covered_symbols(self):
        return super().implicitly_covered_symbols() + [
            'QString',  # Through AbstractTag
        ]

    def gen_key_informational_methods(self, cpp_code, public_block, protected_block):
        with cpp_code.method(
            self.class_name(), 'tagName', 'QString', const=True, virtual=True, declare_in=public_block
        ) as method:
            method.ret('{0}', cpp_string_literal(self._tag_name_for_display()))

        public_block.nl()

        super().gen_key_informational_methods(cpp_code, public_block, protected_block)

    def _pre_deserialize_fields_hook(self, method):
        if self.tag_type == TagContentItemType.STANDARD:
            method \
                .if_block('!must_deserialize(mut_stream, bool)') \
                .ret('make_unique<{0}>({1})', self.class_name(), '' if self.has_mandatory_fields() else 'false')
            return
        elif self.tag_type == TagContentItemType.SYMMETRIC:
            method.declare_var('bool', 'open', 'must_deserialize(mut_stream, bool)')
            return
        elif self.tag_type == TagContentItemType.SELF_CLOSING:
            return

        assert False, 'Unsupported tag_type: {0}'.format(self.tag_type)

    def _extra_deserialization_constructor_params(self):
        if self.tag_type == TagContentItemType.STANDARD:
            return [] if self.has_mandatory_fields() else ['true']
        elif self.tag_type == TagContentItemType.SYMMETRIC:
            return ['open']
        elif self.tag_type == TagContentItemType.SELF_CLOSING:
            return []

        assert False, 'Unsupported tag_type: {0}'.format(self.tag_type)

    def gen_serialize_methods(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            self._serialize_attrs_method_name(),
            'void',
            ParamInfo(type='QDataStream&', name='mut_stream', unused=(len(self.fields) == 0)),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            method.content_if_empty.line_comment('No fields to serialize')
            self.gen_serialize_field_code(method, 'mut_stream', self.fields)

    def _serialize_attrs_method_name(self):
        return 'serialize' + ('OpenTag' if self.tag_type == TagContentItemType.STANDARD else '') + 'AttributesToStream'

    def gen_debug_write_methods(self, cpp_code, _public_block, protected_block):
        with cpp_code.method(
            self.class_name(),
            self._debug_write_method_name(),
            'void',
            ParamInfo(type='QDebug', name='stream', unused=(len(self.fields) == 0)),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                method.content_if_empty.line_comment('No fields to write')
                self.gen_debug_write_field_code(method, 'stream', self.fields)

    def _debug_write_method_name(self):
        return 'write' + ('OpenTag' if self.tag_type == TagContentItemType.STANDARD else '') + 'AttributesToDebugStream'

    def _tag_name_for_display(self):
        if self.tag_name_override is not None:
            return self.tag_name_override

        return self._name[:-3] if self._name.endswith('Tag') else self._name
