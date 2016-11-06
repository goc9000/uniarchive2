# build_assistant/autogen/raw_events/AbstractEventCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import RawEventConfig
from build_assistant.autogen.GenericPolymorphicCodeGenerator import GenericPolymorphicCodeGenerator
from build_assistant.autogen.raw_events.constants import SUBTYPE_ENUM
from build_assistant.autogen.raw_events.EventFieldCodeGenerator import EventFieldCodeGenerator


class AbstractEventCodeGenerator(GenericPolymorphicCodeGenerator):
    def __init__(self, event_config, autogen_core):
        assert isinstance(event_config, RawEventConfig), 'Augmented object should be RawEventConfig'

        GenericPolymorphicCodeGenerator.__init__(
            self, event_config, autogen_core, field_augment=EventFieldCodeGenerator
        )

    def subtype_enum(self):
        return SUBTYPE_ENUM

    def gen_key_informational_methods(self, cpp_code, public_block):
        self.gen_event_name_method(cpp_code, public_block)

    def gen_event_name_method(self, cpp_code, public_block):
        raise NotImplementedError

    def gen_debug_write_details_method(self, cpp_code, block):
        raise NotImplementedError
