# build_assistant/autogen/raw_events/gen_main.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import EnumConfig, EnumValue
from build_assistant.autogen.raw_events.constants import BASE_EVENTS_PATH, SUBTYPE_ENUM, BASE_EVENT_CLASS
from build_assistant.autogen.raw_events.common import event_class_name, event_subtype_value
from build_assistant.autogen.raw_events.BaseEventCodeGenerator import BaseEventCodeGenerator
from build_assistant.autogen.raw_events.EventCodeGenerator import EventCodeGenerator


def autogen_raw_events_index(autogen_config):
    yield BASE_EVENTS_PATH, BASE_EVENT_CLASS

    for path, name, _ in autogen_config.raw_events:
        yield BASE_EVENTS_PATH.append(path), event_class_name(name)


def autogen_raw_events_subtype_enum(autogen_config):
    return BASE_EVENTS_PATH, SUBTYPE_ENUM, EnumConfig(
        values=[
            EnumValue(
                text=name,
                constant=event_subtype_value(name),
                int_value=None,
                comment=None,
            ) for _, name, _ in sorted(autogen_config.raw_events, key=lambda tup: tup[0].add(tup[1]).to_text())
        ],
        internal_comment=None,
        underlying_type_override=None,
    )


def gen_raw_events(autogen_config, autogen_core):
    base_event_generator = BaseEventCodeGenerator(autogen_config.base_raw_event, autogen_core)

    cpp_source, base_event_h = autogen_core.new_pair(BASE_EVENTS_PATH, base_event_generator.class_name())

    base_event_generator.gen_code(cpp_source, base_event_h)

    for rel_path, name, event_config in autogen_config.raw_events:
        event_generator = EventCodeGenerator(name, event_config, autogen_core, base_config=base_event_generator)

        cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH.append(rel_path), event_generator.class_name())
        h_source.cover_symbols_from(base_event_h)

        event_generator.gen_code(cpp_source, h_source)
