# build_assistant/autogen_common.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.VirtualPath import VirtualPath


HASH_LINE_PREFIX = ' * Codegen hash (sha256):'


def get_full_autogen_raw_event_path_and_name(path, name):
    return VirtualPath(['intermediate_format', 'events']).append(path), 'Raw' + name + 'Event'
