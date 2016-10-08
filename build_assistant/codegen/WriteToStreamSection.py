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

    indent_level = None

    stream_name = None
    items = None

    def __init__(self, source, indent_level, stream_name, items=None):
        super().__init__(source)

        self.indent_level = indent_level

        self.stream_name = stream_name
        self.items = items or list()

    def add_item(self, item):
        self.items.append(item)

    def gen_items(self):
        if len(self.items) == 0:
            return

        partial_line = self.stream_name

        for item in self.items:
            if isinstance(item, tuple) or isinstance(item, list):
                item_text = ' << '.join(item)
            else:
                item_text = item

            if not self._line_fits(partial_line + ' << ' + item_text + ';'):
                yield self._render_line(partial_line + ';')

                partial_line = self.stream_name

            partial_line += ' << ' + item_text

        yield self._render_line(partial_line + ';')

    def _line_fits(self, line):
        return self.indent_level * self.source.core.codegen_cfg.indent_size + len(line) \
               <= self.source.core.codegen_cfg.gutter_width

    def _render_line(self, line):
        return ' ' * (self.indent_level * self.source.core.codegen_cfg.indent_size) + line
