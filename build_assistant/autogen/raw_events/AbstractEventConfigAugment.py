# build_assistant/autogen/raw_events/AbstractEventConfigAugment.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.AutoGenConfig import RawEventConfig
from build_assistant.autogen.GenericPolymorphicAugment import GenericPolymorphicAugment
from build_assistant.autogen.raw_events.EventFieldAugment import EventFieldAugment


class AbstractEventConfigAugment(GenericPolymorphicAugment):
    def __init__(self, event_config, autogen_core):
        assert isinstance(event_config, RawEventConfig), 'Augmented object should be RawEventConfig'

        GenericPolymorphicAugment.__init__(self, event_config, autogen_core, field_augment=EventFieldAugment)

    def gen_subtype_method(self, cpp_code, block):
        raise NotImplementedError

    def gen_event_name_method(self, cpp_code, block):
        raise NotImplementedError

    def gen_debug_write_details_method(self, cpp_code, block):
        raise NotImplementedError
