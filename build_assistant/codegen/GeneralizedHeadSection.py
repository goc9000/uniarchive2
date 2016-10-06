# build_assistant/codegen/GeneralizedHeadSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractCodeSectionWithContent import AbstractCodeSection


class GeneralizedHeadSection(AbstractCodeSection):
    indent_level = None

    inherits = None

    def __init__(
        self,
        source,
        indent_level,
        head,
        params=None,
        param_separator=',',
        inherits=None,
        decorations=None,
        closer=';'
    ):
        super().__init__(source)

        self.indent_level = indent_level

        self.head = head
        self.params = params
        self.param_separator = param_separator
        self.inherits = inherits
        self.decorations = decorations
        self.closer = closer

    def gen_items(self):
        if self.inherits is not None and len(self.inherits) == 0:
            self.inherits = None

        # First, try one-liner
        one_line = self.head
        if self.params is not None:
            one_line += '(' + (self.param_separator + ' ').join(self.params) + ')'
        one_line += self.decorations or ''
        without_inherits = one_line
        if self.inherits is not None:
            one_line += ' : ' + ', '.join(self.inherits)
        one_line += self.closer

        if self._line_fits(one_line):
            yield self._render_line(one_line)
            return

        if self.inherits is not None and self._line_fits(without_inherits):  # Then, try to break at the inherits
            yield self._render_line(without_inherits)
            inherits_base = ' '
        elif self.params is not None:  # Try to break at the params
            yield self._render_line(self.head + '(')

            for index, param in enumerate(self.params):
                yield self._render_line(
                    param + (self.param_separator if index < len(self.params) - 1 else ''),
                    self.indent_level + 1
                )

            inherits_base = ')' + (self.decorations or '')

            if self.inherits is None:
                yield self._render_line(inherits_base + self.closer)
                return
        else:
            assert False, "Head too long: {0}".format(self.head)

        line = inherits_base + ' :'

        for index, item in enumerate(self.inherits):
            item_and_sep = item + (',' if index < len(self.inherits) - 1 else self.closer)
            try_line = line + ' ' + item_and_sep

            if not self._line_fits(try_line):
                yield self._render_line(line)
                line = ' ' * (len(inherits_base) + 3) + item_and_sep
            else:
                line = try_line

        yield self._render_line(line)

    def _line_fits(self, line):
        return self.indent_level * self.source.core.codegen_cfg.indent_size + len(line) \
               <= self.source.core.codegen_cfg.gutter_width

    def _render_line(self, line, indent_level=None):
        return ' ' * ((indent_level or self.indent_level) * self.source.core.codegen_cfg.indent_size) + line
