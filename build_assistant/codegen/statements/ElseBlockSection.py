# build_assistant/codegen/statements/ElseBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.AbstractBlockSection import AbstractBlockSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin
from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection
from build_assistant.util.empty_generator import empty_generator


class ElseBlockSection(AbstractBlockSection, ProceduralCodeMixin):
    def __init__(self, source):
        super().__init__(source)

    def _gen_header_items(self, indent_level):
        header_line = IndentedCodeSection(self.source, -1)
        header_line.line('} else {')
        yield header_line

    def _gen_items(self, indent_level):
        return self._gen_content_items()

    def _gen_footer_items(self, indent_level):
        return empty_generator()
