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


def filter_lines(lines):
    """Collapses multiple empty lines into one, and filters out trailing empty lines"""
    has_empty_line = False
    for line in lines:
        if line != '':
            if has_empty_line:
                yield ''
                has_empty_line = False
            yield line
        else:
            has_empty_line = True
