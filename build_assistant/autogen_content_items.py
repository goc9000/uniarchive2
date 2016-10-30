# build_assistant/autogen_content_items.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.AutoGenConfig import ContentItemTagConfig, EnumConfig, EnumValue
from build_assistant.autogen.content_items.content_item_gen_constants import BASE_CONTENT_ITEMS_PATH, SUBTYPE_ENUM
from build_assistant.autogen.content_items.ContentItemConfigAugment import ContentItemConfigAugment
from build_assistant.autogen.content_items.ContentItemTagConfigAugment import ContentItemTagConfigAugment
from build_assistant.util.grammar import camelcase_to_underscore


def autogen_content_items_subtype_enum(autogen_config):
    return BASE_CONTENT_ITEMS_PATH, SUBTYPE_ENUM, EnumConfig(
        values=[
            EnumValue(
                text=name,
                constant=camelcase_to_underscore(name).upper(),
                int_value=None,
                comment=None,
            ) for _, name, _ in sorted(autogen_config.content_items, key=lambda tup: tup[0].add(tup[1]).to_text())
        ],
        internal_comment=None,
    )


def gen_content_items(autogen_config, autogen_core):
    for rel_path, name, item_config in autogen_config.content_items:
        if isinstance(item_config, ContentItemTagConfig):
            item_config = ContentItemTagConfigAugment(name, item_config, autogen_core)
        else:
            item_config = ContentItemConfigAugment(name, item_config, autogen_core)

        class_name = item_config.class_name()

        cpp_source, h_source = autogen_core.new_pair(BASE_CONTENT_ITEMS_PATH.append(rel_path), class_name)

        h_source.cover_symbols(item_config.implicitly_covered_symbols())

        item_config.gen_code(cpp_source, h_source)
