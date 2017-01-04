# build_assistant/autogen/raw_events/common.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.util.grammar import camelcase_to_underscore


def event_class_name(event_name):
    """Goes from a logical event name to a corresponding class name that implements the event"""
    return 'Raw' + event_name + 'Event'


def event_subtype_value(event_name):
    """Goes from a logical event name to the corresponding value in the subtype enum"""
    return camelcase_to_underscore(event_name).upper()
