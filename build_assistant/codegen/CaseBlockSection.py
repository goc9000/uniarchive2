# build_assistant/codegen/CaseBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractBlockSection import AbstractBlockSection
from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection
from build_assistant.util.empty_generator import empty_generator


class CaseBlockSection(AbstractBlockSection):
    label = None

    def __init__(self, source, label):
        super().__init__(source)

        self.label = label

    def _gen_header_items(self, indent_level):
        yield 'case ' + self.label + ':'

    def _gen_items(self, indent_level):
        content = IndentedCodeSection(self.source, 1)
        content.content_items = list(self._gen_content_items())
        yield content

    def _gen_footer_items(self, indent_level):
        return empty_generator()
