# build_assistant/autogen/common_code.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.util.VirtualPath import VirtualPath


def add_deserialization_headers(source):
    source \
        .include('utils/serialization/deserialization_helpers.h') \
        .use(VirtualPath([source.core.codegen_cfg.base_namespace, 'utils', 'serialization']))
