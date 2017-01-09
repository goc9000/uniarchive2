# build_assistant/autogen/GenericPolymorphicCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import GenericPolymorphicConfig
from build_assistant.autogen.ConstructorInfo import ConstructorInfo
from build_assistant.codegen.special.WriteToStreamSection import WriteToStreamSection
from build_assistant.util.Augment import Augment
from build_assistant.util.VirtualPath import VirtualPath


class GenericPolymorphicCodeGenerator(Augment):
    _core = None

    def __init__(self, poly_config, autogen_core, field_augment=None):
        if field_augment is not None:
            poly_config = poly_config._replace(
                fields=[field_augment(field, autogen_core) for field in poly_config.fields]
            )

        Augment.__init__(self, poly_config, GenericPolymorphicConfig)

        self._core = autogen_core

    def subtype_enum(self):
        raise NotImplementedError

    def subtype_value(self):
        raise NotImplementedError

    def mandatory_base_fields(self):
        raise NotImplementedError

    def class_name(self):
        raise NotImplementedError

    def parent_class(self, no_template=None):
        raise NotImplementedError

    def has_mandatory_fields(self):
        return any(f.is_mandatory() for f in self.fields)

    def has_mandatory_fields_sanity_check(self):
        return any(f.is_mandatory() and f.is_checkable() for f in self.fields)

    def constructors(self):
        base_fields = self.mandatory_base_fields()
        free_fields = self.fields
        parent_constructor = \
            [self.parent_class(no_template=True) + '(' + ', '.join(f.as_rvalue() for f in base_fields) + ')'] \
            if self.parent_class() is not None else []

        maybe_addable_fields = filter(lambda f: f.add_to_constructor, free_fields)
        extra_enabled_fields = set()

        while True:
            inited_fields = [f for f in free_fields if f.name in extra_enabled_fields or f.is_mandatory()]
            extra_fields = [f for f in free_fields if f.name in extra_enabled_fields and not f.is_mandatory()]

            yield ConstructorInfo(
                params=[f.as_param() for f in base_fields + inited_fields],
                subconstructors=parent_constructor + [f.as_initializer() for f in inited_fields],
                init_statements=list(),
                extra_fields=extra_fields,
            )

            # Generate convenience constructor for the first singularizable field
            for index, field_config in enumerate(inited_fields):
                if field_config.maybe_singleton:
                    singularized = field_config.singularized()

                    params = \
                        [f.as_param() for f in base_fields + inited_fields[:index]] + \
                        [singularized.as_param()] + \
                        [f.as_param() for f in inited_fields[index + 1:]]

                    subcons = parent_constructor + \
                              [f.as_initializer() for f in inited_fields[:index]] + \
                              [f.as_initializer() for f in inited_fields[index + 1:]]

                    yield ConstructorInfo(
                        params=params,
                        subconstructors=subcons,
                        init_statements=[
                            '{0}.push_back({1});'.format(field_config.name, singularized.as_rvalue())
                        ],
                        extra_fields=extra_fields,
                    )
                    break

            field_config = next(maybe_addable_fields, None)
            if field_config is None:
                break

            extra_enabled_fields.add(field_config.name)

    def class_inherits(self):
        return [self.parent_class()] if self.parent_class() is not None else None

    def gen_code(self, cpp_source, h_source):
        with h_source.code.struct_block(self.class_name(), inherits=self.class_inherits()) as struct_block:
            public_block = struct_block.public_block()
            protected_block = struct_block.protected_block()
            private_block = struct_block.private_block()

        self.gen_base_public_declarations(cpp_source.code, public_block, private_block)
        self.gen_key_informational_methods(cpp_source.code, public_block)
        public_block.nl()

        self.gen_deserialize_methods(cpp_source.code, public_block, protected_block)
        self.gen_serialize_methods(cpp_source.code, protected_block)
        public_block.nl()
        protected_block.nl()

        self.gen_debug_write_methods(cpp_source.code, public_block, protected_block)
        public_block.nl()
        protected_block.nl()

    def gen_base_public_declarations(self, cpp_code, public_block, private_block):
        self.gen_field_declarations(public_block)
        self.gen_constructors(cpp_code, public_block)
        self.gen_mandatory_fields_sanity_check_method(cpp_code, private_block)
        public_block.nl()
        self.gen_subtype_method(cpp_code, public_block)
        public_block.nl()

    def gen_field_declarations(self, block):
        if len(self.fields) > 0:
            for index, field in enumerate(self.fields):
                if index in self.field_breaks:
                    block.nl()

                if field.doc is not None:
                    block.doc_comment(field.doc)

                block.field(*field.as_field_decl())

            block.nl()

    def gen_constructors(self, cpp_code, struct_block):
        for ctor_info in self.constructors():
            with cpp_code.constructor(
                 self.class_name(), *ctor_info.params, inherits=ctor_info.subconstructors, declare_in=struct_block
            ) as cons:
                for line in ctor_info.init_statements:
                    cons.line(line)

                if self.has_mandatory_fields_sanity_check():
                    cons.code_line('sanityCheckMandatoryParameters()')

                for field in ctor_info.extra_fields:
                    field.gen_param_check(cons, disambiguate=True)

    def gen_mandatory_fields_sanity_check_method(self, cpp_code, private_block):
        if self.has_mandatory_fields_sanity_check():
            with cpp_code.method(
                self.class_name(), 'sanityCheckMandatoryParameters', 'void', const=True, declare_in=private_block
            ) as method:
                for field in self.fields:
                    if field.is_mandatory() and field.is_checkable():
                        field.gen_param_check(method)

    def gen_subtype_method(self, cpp_code, public_block):
        with cpp_code.method(
            self.class_name(), 'subType', self.subtype_enum(), const=True, virtual=True, declare_in=public_block
        ) as method:
            method.ret('{0}::{1}', self.subtype_enum(), self.subtype_value())

    def gen_key_informational_methods(self, cpp_code, public_block):
        pass  # Nothing by default

    def gen_deserialize_methods(self, cpp_code, public_block, protected_block):
        pass  # Nothing by default

    def gen_serialize_methods(self, cpp_code, protected_block):
        pass  # Nothing by default

    def gen_debug_write_methods(self, cpp_code, public_block, protected_block):
        pass  # Nothing by default

    def gen_serialize_field_code(self, method, fields):
        regular_fields_section = None
        for field_config in fields:
            if field_config.is_regular_for_serialize():
                if regular_fields_section is None:
                    regular_fields_section = method.subsection(WriteToStreamSection(method.source, 'mut_stream'))

                regular_fields_section.add_item(field_config.as_serialize_rvalue(method))
            else:
                regular_fields_section = None

                field_config.gen_irregular_serialize_code(method)

    def gen_debug_write_field_code(self, method, fields):
        regular_fields_section = None

        for field_config in fields:
            if field_config.is_regular_for_debug_write():
                if regular_fields_section is None:
                    regular_fields_section = method.subsection(WriteToStreamSection(method.source, 'stream'))

                regular_fields_section.add_item(
                    (field_config.debug_write_header(), field_config.as_print_rvalue(method))
                )
            else:
                regular_fields_section = None

                field_config.gen_irregular_debug_write_code(method)

    def add_deserialization_headers(self, source):
        source \
            .include('utils/serialization/deserialization_helpers.h') \
            .use(VirtualPath([source.core.codegen_cfg.base_namespace, 'utils', 'serialization']))
