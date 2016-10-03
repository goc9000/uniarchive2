# build_assistant/codegen_utils.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import json


def cpp_string_literal(text):
    return json.dumps(text)  # Eh, probably works ok in most situations
