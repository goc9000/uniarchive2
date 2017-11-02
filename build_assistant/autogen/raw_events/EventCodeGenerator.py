# build_assistant/autogen/raw_events/EventCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.AbstractEventCodeGenerator import AbstractEventCodeGenerator
from build_assistant.autogen.raw_events.constants import BASE_EVENT_CLASS
from build_assistant.autogen.raw_events.common import event_class_name, event_subtype_value
from build_assistant.autogen.common_code import add_deserialization_headers
from build_assistant.codegen.ParamInfo import ParamInfo


class EventCodeGenerator(AbstractEventCodeGenerator):
    _name = None
    _base_config = None

    def __init__(self, name, event_config, autogen_core, base_config=None):
        AbstractEventCodeGenerator.__init__(self, event_config, autogen_core)
        self._name = name
        self._base_config = base_config

    def subtype_value(self):
        return event_subtype_value(self._name)

    def mandatory_base_fields(self):
        return [f for f in self._base_config.fields if f.is_mandatory()]

    def class_name(self):
        return event_class_name(self._name)

    def parent_class(self, no_template=False):
        if self.fail_reason_enum is None:
            return BASE_EVENT_CLASS
        elif no_template:
            return 'RawFailableEvent'
        else:
            return 'RawFailableEvent<{0}>'.format(self.fail_reason_enum)

    def gen_event_name_method(self, cpp_code, public_block):
        if self.custom_name_method:
            with cpp_code.method(
                self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=public_block
            ) as method:
                method.custom_section('Name method')

    def gen_visit_subjects_methods(self, cpp_code, _public_block, protected_block):
        self.gen_visit_subjects_impl_method(cpp_code, protected_block)

    def gen_visit_subjects_impl_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(), 'visitSubjectsImpl',
            'void',
            ParamInfo(type='IApparentSubjectVisitor&', name='visitor', unused=True),  # Assume always unused for now
            declare_in=protected_block
        ) as method:
            method.content_if_empty.line_comment('No fields to visit')

            self.gen_visit_subjects_field_code(method, 'visitor', self.fields)

    def gen_deserialize_methods(self, cpp_code, public_block, protected_block):
        add_deserialization_headers(cpp_code.source)

        with cpp_code.method(
            self.class_name(), 'deserializeFromStream', 'CEDE({0})'.format(self.class_name()),
            ('QDataStream&', 'mut_stream'), ParamInfo(type='bool', name='skip_type', default_value='false'),
            static=True, declare_in=public_block
        ) as method:
            method.code_line(
                'maybeDeserializeType(skip_type, mut_stream, {0}::{1})', self.subtype_enum(), self.subtype_value()
            ).nl()

            for field in self._base_config.fields:
                field.gen_deserialize_to_var(method, 'mut_stream')

            if self.fail_reason_enum:
                method.declare_var(
                    'optional<{0}>'.format(self.fail_reason_enum),
                    'fail_reason',
                    'must_deserialize(mut_stream, optional<{0}>)'.format(self.fail_reason_enum)
                )

            for field in self.fields:
                field.gen_deserialize_to_var(method, 'mut_stream')

            method.nl().call(
                'unique_ptr<{0}> event = make_unique<{0}>'.format(self.class_name()),
                *[
                    field.as_rvalue()
                    for field in self._base_config.fields + self.fields
                    if field.is_mandatory()
                ]
            )

            if self.fail_reason_enum:
                method.code_line('event->reasonFailed = fail_reason')

            for field in self._base_config.fields + self.fields:
                if not field.is_mandatory():
                    method.code_line('event->{0} = {1}', field.name, field.as_rvalue())

            method.nl().ret('event')

    def gen_serialize_methods(self, cpp_code, protected_block):
        self.gen_serialize_details_method(cpp_code, protected_block)

    def gen_serialize_details_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            'serialize' + ('FailableEvent' if self.fail_reason_enum is not None else '') + 'DetailsToStream',
            'void',
            ParamInfo(type='QDataStream&', name='mut_stream', unused=(len(self.fields) == 0)),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            method.content_if_empty.line_comment('No fields to serialize')
            self.gen_serialize_field_code(method, 'mut_stream', self.fields)

    def gen_debug_write_methods(self, cpp_code, _public_block, protected_block):
        self.gen_debug_write_details_method(cpp_code, protected_block)

    def gen_debug_write_details_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            'write' + ('FailableEvent' if self.fail_reason_enum is not None else '') + 'DetailsToDebugStream',
            'void',
            ParamInfo(type='QDebug', name='stream', unused=(len(self.fields) == 0)),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                method.content_if_empty.line_comment('No fields to write')
                self.gen_debug_write_field_code(method, 'stream', self.fields)
