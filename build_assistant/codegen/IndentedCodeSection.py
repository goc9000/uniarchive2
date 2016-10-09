# build_assistant/codegen/IndentedCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.GeneralCodeSection import GeneralCodeSection


class IndentedCodeSection(GeneralCodeSection):
    indent_delta = None

    def __init__(self, source, indent_delta):
        super().__init__(source)

        self.indent_delta = indent_delta

    def gen_lines(self, indent_level):
        return super().gen_lines(indent_level + self.indent_delta)
