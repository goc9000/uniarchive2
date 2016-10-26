# build_assistant/GenericPolymorphicAugment.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from collections import namedtuple

from build_assistant.AutoGenConfig import GenericPolymorphicConfig
from build_assistant.codegen.special.WriteToStreamSection import WriteToStreamSection
from build_assistant.util.Augment import Augment
from build_assistant.util.grammar import classname_to_varname


ConstructorInfo = namedtuple('ConstructorInfo', ['params', 'subconstructors', 'init_statements', 'extra_fields'])


class GenericPolymorphicAugment(Augment):
    _core = None

    def __init__(self, poly_config, autogen_core, field_augment=None):
        if field_augment is not None:
            poly_config = poly_config._replace(
                fields=[field_augment(field, autogen_core) for field in poly_config.fields]
            )

        Augment.__init__(self, poly_config, GenericPolymorphicConfig)

        self._core = autogen_core

    def mandatory_base_fields(self):
        raise NotImplementedError

    def class_name(self):
        raise NotImplementedError

    def parent_class(self, no_template=None):
        raise NotImplementedError

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

    def gen_code(self, cpp_source, h_source):
        raise NotImplementedError

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

    def gen_mandatory_fields_sanity_check_method(self, cpp_code):
        if self.has_mandatory_fields_sanity_check():
            with cpp_code.method(self.class_name(), 'sanityCheckMandatoryParameters', 'void', const=True) as method:
                for field in self.fields:
                    if field.is_mandatory() and field.is_checkable():
                        field.gen_param_check(method)

    def gen_private_block(self, struct):
        if self.has_private_block():
            with struct.private_block() as block:
                block.declare_fn('sanityCheckMandatoryParameters', 'void', const=True)

    def gen_debug_write_field_code(self, method, fields):
        def write_regular_field(block, regular_fields_section, field_config):
            if regular_fields_section is None:
                regular_fields_section = block.subsection(WriteToStreamSection(block.source, 'stream'))

            regular_fields_section.add_item((field_config.debug_write_header(), field_config.as_print_rvalue(block)))

            return regular_fields_section

        def write_irregular_field(block, field_config):
            if field_config.is_optional:
                with block.if_block(field_config.name, nl_after=False) as b:
                    write_irregular_field2(b, field_config)
            else:
                write_irregular_field2(block, field_config)

        def write_irregular_field2(block, field_config):
            if field_config.maybe_singleton:
                rvalue = field_config.as_print_rvalue(block)

                with block.if_block('{0} == 1'.format(field_config.as_subfield_value('size()')), nl_after=False) as b:
                    b.code_line(
                        'stream << {0} << {1}',
                        field_config.singularized().debug_write_header(),
                        field_config.as_subfield_value('front()'),
                    )
                    with b.else_block() as e:
                        e.code_line('stream << {0} << {1}', field_config.debug_write_header(), rvalue)
            else:
                write_irregular_field3(block, field_config)

        def write_irregular_field3(block, field_config):
            block.code_line(
                'stream << {0} << {1}', field_config.debug_write_header(), field_config.as_print_rvalue(block)
            )

        regular_fields_section = None

        for field_config in fields:
            # First, write regular fields
            if not (field_config.is_optional or field_config.maybe_singleton):
                regular_fields_section = write_regular_field(method, regular_fields_section, field_config)
                continue

            regular_fields_section = None

            write_irregular_field(method, field_config)

    def gen_debug_write_operator(self, cpp_code, h_code):
        varname = classname_to_varname(self.class_name())

        with cpp_code.function(
            'operator<< ', 'QDebug', ('QDebug', 'stream'), ('CPTR({0})'.format(self.class_name()), varname),
            declare_in=h_code
        ) as method:
            method \
                .code_line('{0}->writeToDebugStream(stream)', varname) \
                .ret('stream')

    def has_private_block(self):
        return self.has_mandatory_fields_sanity_check()

    def has_mandatory_fields(self):
        return any(f.is_mandatory() for f in self.fields)

    def has_mandatory_fields_sanity_check(self):
        return any(f.is_mandatory() and f.is_checkable() for f in self.fields)
