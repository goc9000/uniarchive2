# build_assistant/autogen/content_items/common.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.util.grammar import camelcase_to_underscore


def content_item_class_name(item_name):
    """Goes from a logical content item name to a corresponding class name that implements the item"""
    return item_name


def content_item_subtype_value(item_name):
    """Goes from a logical content item name to the corresponding value in the subtype enum"""
    return camelcase_to_underscore(item_name).upper()
