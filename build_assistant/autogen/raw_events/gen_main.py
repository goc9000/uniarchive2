# build_assistant/autogen/raw_events/gen_main.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import EnumConfig, EnumValue
from build_assistant.autogen.raw_events.constants import BASE_EVENTS_PATH, SUBTYPE_ENUM
from build_assistant.autogen.raw_events.BaseEventCodeGenerator import BaseEventCodeGenerator
from build_assistant.autogen.raw_events.EventCodeGenerator import EventCodeGenerator
from build_assistant.util.grammar import camelcase_to_underscore


def autogen_raw_events_index(autogen_config):
    yield BASE_EVENTS_PATH, 'RawEvent'

    for path, name, _ in autogen_config.raw_events:
        yield BASE_EVENTS_PATH.append(path), 'Raw' + name + 'Event'

    return []


def autogen_raw_events_subtype_enum(autogen_config):
    return BASE_EVENTS_PATH, SUBTYPE_ENUM, EnumConfig(
        values=[
            EnumValue(
                text=name,
                constant=camelcase_to_underscore(name).upper(),
                int_value=None,
                comment=None,
            ) for _, name, _ in sorted(autogen_config.raw_events, key=lambda tup: tup[0].add(tup[1]).to_text())
        ],
        internal_comment=None,
    )


def gen_raw_events(autogen_config, autogen_core):
    base_event_config = BaseEventCodeGenerator(autogen_config.base_raw_event, autogen_core)

    base_event_h = gen_base_raw_event(base_event_config, autogen_core)

    for rel_path, name, event_config in autogen_config.raw_events:
        event_config = EventCodeGenerator(name, event_config, autogen_core, base_config=base_event_config)

        cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH.append(rel_path), event_config.class_name())
        h_source.cover_symbols_from(base_event_h)

        event_config.gen_code(cpp_source, h_source)


def gen_base_raw_event(base_event_config, autogen_core):
    cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH, base_event_config.class_name())

    base_event_config.gen_code(cpp_source, h_source)

    return h_source
