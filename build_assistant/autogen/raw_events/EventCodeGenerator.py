# build_assistant/autogen/raw_events/EventCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.AbstractEventCodeGenerator import AbstractEventCodeGenerator
from build_assistant.util.grammar import camelcase_to_underscore


class EventCodeGenerator(AbstractEventCodeGenerator):
    _name = None
    _base_config = None

    def __init__(self, name, event_config, autogen_core, base_config=None):
        AbstractEventCodeGenerator.__init__(self, event_config, autogen_core)
        self._name = name
        self._base_config = base_config

    def subtype_value(self):
        return camelcase_to_underscore(self._name).upper()

    def mandatory_base_fields(self):
        return [f for f in self._base_config.fields if f.is_mandatory()]

    def class_name(self):
        return 'Raw' + self._name + 'Event'

    def parent_class(self, no_template=False):
        if self.fail_reason_enum is None:
            return 'RawEvent'
        elif no_template:
            return 'RawFailableEvent'
        else:
            return 'RawFailableEvent<{0}>'.format(self.fail_reason_enum)

    def gen_code_impl(self, cpp_source, h_source, public_block, protected_block, private_block):
        self.gen_base_public_declarations(cpp_source.code, public_block, private_block)
        self.gen_key_informational_methods(cpp_source.code, public_block)
        public_block.nl()
        self.gen_debug_write_methods(cpp_source.code, public_block, protected_block)
        public_block.nl()
        protected_block.nl()

    def gen_event_name_method(self, cpp_code, public_block):
        if self.custom_name_method:
            with cpp_code.method(
                self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=public_block
            ) as method:
                method.custom_section('Name method')

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
