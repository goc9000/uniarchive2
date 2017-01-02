# build_assistant/codegen/statements/switch/DefaultCaseBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.statements.switch.AbstractCaseBlockSection import AbstractCaseBlockSection


class DefaultCaseBlockSection(AbstractCaseBlockSection):
    def _gen_header_items(self, indent_level):
        yield 'default:'
