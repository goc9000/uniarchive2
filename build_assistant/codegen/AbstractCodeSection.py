# build_assistant/codegen/AbstractCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


class AbstractCodeSection:
    source = None

    def __init__(self, source):
        self.source = source

    def gen_lines(self, indent_level):
        return self._gen_lines_for_items(self._gen_items(indent_level), indent_level)

    def _gen_items(self, indent_level):
        raise NotImplementedError

    def _gen_lines_for_items(self, items, indent_level):
        for item in items:
            if isinstance(item, AbstractCodeSection):
                for line in item.gen_lines(indent_level):
                    yield line
            elif item == '':
                yield ''
            else:
                yield ' ' * (indent_level * self.source.core.codegen_cfg.indent_size) + item

    def _line_fits(self, indent_level, line):
        return indent_level * self.source.core.codegen_cfg.indent_size + len(line) \
               <= self.source.core.codegen_cfg.gutter_width

