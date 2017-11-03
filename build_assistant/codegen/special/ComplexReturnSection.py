# build_assistant/codegen/special/ComplexReturnSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.AbstractCodeSection import AbstractCodeSection
from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection


class ComplexReturnSection(AbstractCodeSection):
    """
    This is a section for code that looks like this:

        return item1 OP item2 OP item3;

    or, alternatively, when items don't fit:

        return
          item1 OP item2 OP item3 OP
          item4 OP item5;
    """

    operator = None
    items = None

    def __init__(self, source, operator, items=None):
        super().__init__(source)

        self.operator = operator
        self.items = items or list()

    def add_item(self, item):
        self.items.append(item)

    def _gen_items(self, indent_level):
        assert len(self.items) > 0, 'Complex return needs at least one element'

        one_line_candidate = 'return ' + (' ' + self.operator + ' ').join(self.items) + ';'

        if self._line_fits(indent_level, one_line_candidate):
            yield one_line_candidate
        else:
            yield 'return'

            subsection = IndentedCodeSection(self.source, 1)

            partial_line = ''

            for index, item in enumerate(self.items):
                is_last = index == len(self.items) - 1
                to_add = ('' if partial_line == '' else ' ') + item + (';' if is_last else (' ' + self.operator))

                if self._line_fits(indent_level + 1, partial_line):
                    partial_line += to_add
                else:
                    subsection.content_items.append(partial_line)
                    partial_line = to_add.lstrip()

            subsection.content_items.append(partial_line)

            yield subsection
