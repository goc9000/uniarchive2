# build_assistant/autogen/raw_events/BaseEventCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.AbstractEventCodeGenerator import AbstractEventCodeGenerator
from build_assistant.autogen.raw_events.constants import BASE_EVENT_CLASS, SUBTYPE_ENUM
from build_assistant.codegen.ParamInfo import ParamInfo


class BaseEventCodeGenerator(AbstractEventCodeGenerator):
    def __init__(self, event_config, autogen_core):
        AbstractEventCodeGenerator.__init__(self, event_config, autogen_core)

    def mandatory_base_fields(self):
        return list()

    def class_name(self):
        return BASE_EVENT_CLASS

    def parent_class(self, no_template=None):
        return 'StandardPolymorphic' if no_template else 'StandardPolymorphic<{0}>'.format(self.subtype_enum())

    def interfaces(self):
        return ['IApparentSubjectVisitable']

    def gen_subtype_method(self, _cpp_code, _public_block):
        pass  # Do not generate this method as this is a base class

    def gen_event_name_method(self, cpp_code, block):
        with cpp_code.method(
            self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=block
        ) as method:
            method.ret('name_for_raw_event_sub_type(subType())')

    def gen_visit_subjects_methods(self, cpp_code, public_block, protected_block):
        self._gen_visit_subjects_method(cpp_code, public_block)
        self.gen_visit_subjects_impl_method(cpp_code, protected_block)

    def _gen_visit_subjects_method(self, cpp_code, public_block):
        with cpp_code.method(
            self.class_name(), 'visitSubjects', 'void', ('IApparentSubjectVisitor&', 'visitor'), declare_in=public_block
        ) as method:
            self.gen_visit_subjects_field_code(method, 'visitor', self.fields)

            method.nl().code_line('visitSubjectsImpl(visitor)')

    def gen_visit_subjects_impl_method(self, _cpp_code, protected_block):
        protected_block.declare_method(
            'visitSubjectsImpl', 'void', ('IApparentSubjectVisitor&', 'visitor'),
            virtual=True, abstract=True
        )

    def gen_deserialize_methods(self, cpp_code, public_block, protected_block):
        from build_assistant.autogen.raw_events.gen_main import autogen_raw_events_index

        item_index = autogen_raw_events_index(cpp_code.source.core.autogen_config, include_base=False)

        self.gen_deserialization_manifold(cpp_code, public_block, item_index)

    def gen_serialize_methods(self, cpp_code, protected_block):
        self._gen_serialize_method(cpp_code, protected_block)
        self.gen_serialize_details_method(cpp_code, protected_block)

    def _gen_serialize_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(), 'serializeToStreamImpl', 'void', ('QDataStream&', 'mut_stream'),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            self.gen_serialize_field_code(method, 'mut_stream', self.fields)

            method.nl().code_line('serializeDetailsToStream(mut_stream)')

    def gen_serialize_details_method(self, _cpp_code, protected_block):
        protected_block.declare_method(
            'serializeDetailsToStream', 'void', ('QDataStream&', 'mut_stream'),
            const=True, virtual=True, abstract=True
        )

    def gen_debug_write_methods(self, cpp_code, public_block, protected_block):
        self._gen_debug_write_method(cpp_code, public_block)
        self.gen_debug_write_details_method(cpp_code, protected_block)

    def _gen_debug_write_method(self, cpp_code, public_block):
        time_field = None
        index_field = None

        remaining_fields = []
        for field_config in self.fields:
            if field_config.name == 'timestamp' and time_field is None:
                time_field = field_config
            elif field_config.name.startswith('index') and index_field is None:
                index_field = field_config
            else:
                remaining_fields.append(field_config)

        with cpp_code.method(
            self.class_name(), 'writeToDebugStream', 'void', ('QDebug', 'stream'), const=True, declare_in=public_block
        ) as method:
            method \
                .declare_var('QDebugStateSaver', 'saver(stream)') \
                .code_line('stream.nospace()').nl() \
                .code_line('stream << "#" << {0} << " "', index_field.as_print_rvalue(cpp_code)) \
                .code_line('stream << "[" << {0} << "] "', time_field.as_print_rvalue(cpp_code)).nl() \
                .code_line('stream << QP(eventName())') \
                .code_line('writeDetailsToDebugStream(stream)').nl()
            cpp_code.source.use_symbol('QP')

            self.gen_debug_write_field_code(method, 'stream', remaining_fields)

    def gen_debug_write_details_method(self, _cpp_code, protected_block):
        protected_block.declare_method(
            'writeDetailsToDebugStream', 'void', ('QDebug', 'stream'),
            const=True, virtual=True, abstract=True
        )