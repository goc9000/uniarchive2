# build_assistant/autogen/raw_events/EventCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.constants import SUBTYPE_ENUM
from build_assistant.autogen.raw_events.AbstractEventCodeGenerator import AbstractEventCodeGenerator
from build_assistant.util.grammar import camelcase_to_underscore


class EventCodeGenerator(AbstractEventCodeGenerator):
    _name = None
    _base_config = None

    def __init__(self, name, event_config, autogen_core, base_config=None):
        AbstractEventCodeGenerator.__init__(self, event_config, autogen_core)
        self._name = name
        self._base_config = base_config

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
        with public_block as block:
            self.gen_field_declarations(block)
            self.gen_constructors(cpp_source.code, block)
            self.gen_mandatory_fields_sanity_check_method(cpp_source.code, private_block)

            block.nl()
            self.gen_subtype_method(cpp_source.code, block)
            block.nl()
            self.gen_event_name_method(cpp_source.code, block)

        self.gen_debug_write_details_method(cpp_source.code, protected_block)

    def gen_subtype_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(), 'subType', SUBTYPE_ENUM, const=True, virtual=True, declare_in=struct_block
        ) as method:
            method.ret('{0}::{1}', SUBTYPE_ENUM, camelcase_to_underscore(self._name).upper())

    def gen_event_name_method(self, cpp_code, struct_block):
        if self.custom_name_method:
            with cpp_code.method(
                self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=struct_block
            ) as method:
                method.custom_section('Name method')

    def gen_debug_write_details_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(),
            'write' + ('FailableEvent' if self.fail_reason_enum is not None else '') + 'DetailsToDebugStream',
            'void',
            ('QDebug' + (' UNUSED' if len(self.fields) == 0 else ''), 'stream'),
            const=True, virtual=True, declare_in=struct_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                self.gen_debug_write_field_code(method, self.fields)
