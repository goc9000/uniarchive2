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
    auto_collapse = None  # Return no lines if content is empty
    content_if_empty = None

    def __init__(self, source, auto_collapse=False):
        super().__init__(source)

        from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection

        self.auto_collapse = auto_collapse
        self.content_if_empty = IndentedCodeSection(source, 1)

    def gen_lines(self, indent_level):
        if not self.auto_collapse:
            for line in self._gen_lines_for_items(self._gen_header_items(indent_level), indent_level):
                yield line

            is_content_empty = True
            for line in filter_lines(self._gen_lines_for_items(self._gen_items(indent_level), indent_level)):
                yield line
                is_content_empty = False

            if is_content_empty:
                for line in filter_lines(self.content_if_empty.gen_lines(indent_level)):
                    yield line

            for line in self._gen_lines_for_items(self._gen_footer_items(indent_level), indent_level):
                yield line
        else:
            header_emitted = False

            for line in filter_lines(self._gen_lines_for_items(self._gen_items(indent_level), indent_level)):
                if not header_emitted:
                    header_emitted = True

                    for header_line in self._gen_lines_for_items(self._gen_header_items(indent_level), indent_level):
                        yield header_line

                yield line

            if header_emitted:
                for line in self._gen_lines_for_items(self._gen_footer_items(indent_level), indent_level):
                    yield line

    def _gen_header_items(self, indent_level):
        raise NotImplementedError

    def _gen_items(self, indent_level):
        return self._gen_content_items()

    def _gen_footer_items(self, indent_level):
        raise NotImplementedError
