# build_assistant/codegen/codegen_utils.py
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
    """Collapses multiple empty lines into one, and filters out leading & trailing empty lines"""
    has_empty_line = False
    first_line = True

    for line in lines:
        if line != '':
            if has_empty_line:
                if not first_line:
                    yield ''
                has_empty_line = False

            yield line

            first_line = False
        else:
            has_empty_line = True
