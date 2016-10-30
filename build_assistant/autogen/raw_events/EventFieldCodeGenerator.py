# build_assistant/autogen/raw_events/EventFieldCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import RawEventFieldConfig
from build_assistant.autogen.GenericPolymorphicFieldCodeGenerator import GenericPolymorphicFieldCodeGenerator


class EventFieldCodeGenerator(GenericPolymorphicFieldCodeGenerator):
    def __init__(self, field_config, autogen_core):
        assert isinstance(field_config, RawEventFieldConfig), 'Augmented object should be RawEventFieldConfig'

        GenericPolymorphicFieldCodeGenerator.__init__(self, field_config, autogen_core)
