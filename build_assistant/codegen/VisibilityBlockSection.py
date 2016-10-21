# build_assistant/codegen/VisibilityBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.AbstractBlockSection import AbstractBlockSection
from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection
from build_assistant.codegen.mixins.DeclareClassStuffMixin import DeclareClassStuffMixin
from build_assistant.util.empty_generator import empty_generator


class VisibilityBlockSection(AbstractBlockSection, DeclareClassStuffMixin):
    kind = None

    def __init__(self, source, kind):
        super().__init__(source)

        self.kind = kind

    def _gen_header_items(self, indent_level):
        yield ''

        header_line = IndentedCodeSection(self.source, -1)
        header_line.line(self.kind + ':')
        yield header_line

    def _gen_items(self, indent_level):
        return self._gen_content_items()

    def _gen_footer_items(self, indent_level):
        return empty_generator()
