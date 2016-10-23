# build_assistant/codegen/abstract/AbstractCodeSectionWithContent.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.AbstractCodeSection import AbstractCodeSection
from build_assistant.autogen_common import BEGIN_CUSTOM_SECTION_LINE_PREFIX, END_CUSTOM_SECTION_LINE_PREFIX


class AbstractCodeSectionWithContent(AbstractCodeSection):
    content_items = None

    def __init__(self, source):
        super().__init__(source)

        self.content_items = list()

    def _gen_content_items(self):
        for item in self.content_items:
            yield item

    # Basic constructs applicable to any section

    def line(self, line):
        self.content_items.append(line.rstrip())
        return self

    def nl(self):
        return self.line('')

    def subsection(self, section):
        self.content_items.append(section)
        return section

    def line_comment(self, comment):
        for line in comment.split("\n"):
            self.line('// ' + line)
        return self

    def doc_comment(self, comment):
        self.line('/**')
        for line in comment.split("\n"):
            self.line(' * ' + line)
        self.line(' */')

        return self

    def custom_section(self, name):
        self.source.custom_sections.add(name)

        self.line(BEGIN_CUSTOM_SECTION_LINE_PREFIX + name)
        self.line(END_CUSTOM_SECTION_LINE_PREFIX + name)

        return self

    def _generalized_head(self, *args, **kwargs):
        from build_assistant.codegen.abstract.GeneralizedHeadSection import GeneralizedHeadSection

        self.content_items.append(GeneralizedHeadSection(self.source, *args, **kwargs))

        return self
