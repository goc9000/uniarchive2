# build_assistant/codegen/AbstractCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


class AbstractCodeSection:
    source = None

    def __init__(self, source):
        self.source = source

    def gen_lines(self):
        raise NotImplementedError
