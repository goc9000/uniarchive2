# build_assistant/autogen/ConstructorInfo.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from collections import namedtuple


ConstructorInfo = namedtuple('ConstructorInfo', ['params', 'subconstructors', 'init_statements', 'extra_fields'])
