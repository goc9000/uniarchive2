# build_assistant/codegen/abstract/AbstractBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.codegen_utils import filter_lines
from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent


class AbstractBlockSection(AbstractCodeSectionWithContent):

    def __init__(self, source):
        super().__init__(source)

    def gen_lines(self, indent_level):
        for line in self._gen_lines_for_items(self._gen_header_items(indent_level), indent_level):
            yield line
        for line in filter_lines(self._gen_lines_for_items(self._gen_items(indent_level), indent_level)):
            yield line
        for line in self._gen_lines_for_items(self._gen_footer_items(indent_level), indent_level):
            yield line

    def _gen_header_items(self, indent_level):
        raise NotImplementedError

    def _gen_items(self, indent_level):
        return self._gen_content_items()

    def _gen_footer_items(self, indent_level):
        raise NotImplementedError
