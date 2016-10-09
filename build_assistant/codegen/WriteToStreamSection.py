# build_assistant/codegen/WriteToStreamSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractCodeSection import AbstractCodeSection


class WriteToStreamSection(AbstractCodeSection):
    """
    This is a section for code that looks like this:

        stream << item1 << item2 << item3;

    or, alternatively, when items don't fit:

        stream << item1 << item2;
        stream << item3;
    """

    stream_name = None
    items = None

    def __init__(self, source, stream_name, items=None):
        super().__init__(source)

        self.stream_name = stream_name
        self.items = items or list()

    def add_item(self, item):
        self.items.append(item)

    def gen_items(self, indent_level):
        if len(self.items) == 0:
            return

        partial_line = self.stream_name

        for item in self.items:
            if isinstance(item, tuple) or isinstance(item, list):
                item_text = ' << '.join(item)
            else:
                item_text = item

            if not self._line_fits(indent_level, partial_line + ' << ' + item_text + ';'):
                yield partial_line + ';'

                partial_line = self.stream_name

            partial_line += ' << ' + item_text

        yield partial_line + ';'
