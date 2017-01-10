# build_assistant/autogen/content_items/gen_main.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import TagContentItemConfig, EnumConfig, EnumValue
from build_assistant.autogen.content_items.constants import BASE_CONTENT_ITEMS_PATH, BASE_CONTENT_ITEMS_CLASS, \
    SUBTYPE_ENUM
from build_assistant.autogen.content_items.common import content_item_class_name, content_item_subtype_value
from build_assistant.autogen.content_items.BaseContentItemCodeGenerator import BaseContentItemCodeGenerator
from build_assistant.autogen.content_items.ContentItemCodeGenerator import ContentItemCodeGenerator
from build_assistant.autogen.content_items.TagContentItemCodeGenerator import TagContentItemCodeGenerator

from collections import namedtuple


RawContentItemsIndexEntry = namedtuple('RawEventsIndexEntry', ['item_name', 'path', 'class_name', 'subtype_constant'])


def autogen_content_items_index(autogen_config, include_base=True):
    if include_base:
        yield RawContentItemsIndexEntry(
            item_name=None,
            path=BASE_CONTENT_ITEMS_PATH,
            class_name=BASE_CONTENT_ITEMS_CLASS,
            subtype_constant=None
        )

    for path, name, _ in sorted(autogen_config.content_items, key=lambda tup: tup[0].add(tup[1]).to_text()):
        yield RawContentItemsIndexEntry(
            item_name=name,
            path=BASE_CONTENT_ITEMS_PATH.append(path),
            class_name=content_item_class_name(name),
            subtype_constant=content_item_subtype_value(name)
        )


def autogen_content_items_subtype_enum(autogen_config):
    return BASE_CONTENT_ITEMS_PATH, SUBTYPE_ENUM, EnumConfig(
        values=[
            EnumValue(
                text=item.item_name,
                constant=item.subtype_constant,
                int_value=None,
                comment=None,
            ) for item in autogen_content_items_index(autogen_config, include_base=False)
        ],
        internal_comment=None,
        underlying_type_override=None,
    )


def gen_content_items(autogen_config, autogen_core):
    base_item_generator = BaseContentItemCodeGenerator(autogen_core)

    cpp_source, base_item_h = autogen_core.new_pair(BASE_CONTENT_ITEMS_PATH, base_item_generator.class_name())

    base_item_generator.gen_code(cpp_source, base_item_h)

    for rel_path, name, item_config in autogen_config.content_items:
        if isinstance(item_config, TagContentItemConfig):
            item_generator = TagContentItemCodeGenerator(name, item_config, autogen_core)
        else:
            item_generator = ContentItemCodeGenerator(name, item_config, autogen_core)

        class_name = item_generator.class_name()

        cpp_source, h_source = autogen_core.new_pair(BASE_CONTENT_ITEMS_PATH.append(rel_path), class_name)

        h_source.cover_symbols(item_generator.implicitly_covered_symbols())

        item_generator.gen_code(cpp_source, h_source)
