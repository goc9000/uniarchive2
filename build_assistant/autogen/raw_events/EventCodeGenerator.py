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
from build_assistant.util.grammar import camelcase_to_underscore


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

    def gen_serialize_methods(self, cpp_code, protected_block):
        self.gen_serialize_details_method(cpp_code, protected_block)

    def gen_serialize_details_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            'serialize' + ('FailableEvent' if self.fail_reason_enum is not None else '') + 'DetailsToStream',
            'void',
            ('QDataStream&' + (' UNUSED' if len(self.fields) == 0 else ''), 'mut_stream'),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            self.gen_serialize_field_code(method, self.fields)

    def gen_debug_write_methods(self, cpp_code, _public_block, protected_block):
        self.gen_debug_write_details_method(cpp_code, protected_block)

    def gen_debug_write_details_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(),
            'write' + ('FailableEvent' if self.fail_reason_enum is not None else '') + 'DetailsToDebugStream',
            'void',
            ('QDebug' + (' UNUSED' if len(self.fields) == 0 else ''), 'stream'),
            const=True, virtual=True, declare_in=protected_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                self.gen_debug_write_field_code(method, self.fields)
