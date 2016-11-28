# build_assistant/autogen/enums/gen_main.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.gen_main import autogen_raw_events_subtype_enum
from build_assistant.autogen.content_items.gen_main import autogen_content_items_subtype_enum
from build_assistant.autogen.enums.EnumCodeGenerator import EnumCodeGenerator


def autogen_enums_index(autogen_config):
    for path, name, _ in all_enums(autogen_config):
        yield path, name


def all_enums(autogen_config):
    for item in autogen_config.enums:
        yield item
    for item in synthetic_enums(autogen_config):
        yield item


def synthetic_enums(autogen_config):
    yield autogen_raw_events_subtype_enum(autogen_config)
    yield autogen_content_items_subtype_enum(autogen_config)


def gen_enums(autogen_config, autogen_core):
    for path, name, enum_config in all_enums(autogen_config):
        cpp_source, h_source = autogen_core.new_pair(path, name)

        EnumCodeGenerator(name, enum_config, autogen_core).gen_code(cpp_source, h_source)
