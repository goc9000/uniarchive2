# build_assistant/codegen/statements/switch/CaseBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.statements.switch.AbstractCaseBlockSection import AbstractCaseBlockSection


class CaseBlockSection(AbstractCaseBlockSection):
    labels = None

    def __init__(self, source, *labels):
        super().__init__(source)

        self.labels = labels

    def _gen_header_items(self, indent_level):
        for label in self.labels:
            yield 'case ' + label + ':'
