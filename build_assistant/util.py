# build_assistant/util.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import sys

from textwrap import dedent


def fail(format_str, *args, **kwargs):
    print(dedent(format_str.format(*args, **kwargs)).strip(), file=sys.stderr)
    sys.exit(-1)
