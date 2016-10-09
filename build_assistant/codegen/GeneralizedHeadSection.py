# build_assistant/codegen/GeneralizedHeadSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractCodeSection import AbstractCodeSection
from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection


class GeneralizedHeadSection(AbstractCodeSection):
    """
    A generalized head is a section of code that looks like this:

        HEAD(param1, param2, param3) : inherit1, inherit2, inherit3;

    or, alternatively, when items don't fit:

        HEAD(
          param1,
          param2
          param3
        ) : inherit1, inherit2,
            inherit3;

    Note how this matches constructors, function declarations, but also the head of for loops, class definitions, etc.
    The fully general form is:

        <head> [ "(" <params[0]> <param_separator> params[1] <param_separator> ... ")" ] <decorations>
               [ ":" <inherits[0]> "," <inherits[1]> "," ... ] <closer>
    """

    head = None
    params = None
    param_separator = None
    inherits = None
    decorations = None
    closer = None

    def __init__(
        self,
        source,
        head,
        params=None,
        param_separator=',',
        inherits=None,
        decorations=None,
        closer=';'
    ):
        super().__init__(source)

        self.head = head
        self.params = params
        self.param_separator = param_separator
        self.inherits = inherits
        self.decorations = decorations
        self.closer = closer

    def gen_items(self, indent_level):
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

        if self._line_fits(indent_level, one_line):
            yield one_line
            return

        # Then, try to break at the inherits
        if self.inherits is not None and self._line_fits(indent_level, without_inherits):
            yield without_inherits
            inherits_base = ' '
        elif self.params is not None:  # Try to break at the params
            yield self.head + '('

            params_section = IndentedCodeSection(self.source, 1)
            for index, param in enumerate(self.params):
                params_section.line(param + (self.param_separator if index < len(self.params) - 1 else ''))
            yield params_section

            inherits_base = ')' + (self.decorations or '')

            if self.inherits is None:
                yield inherits_base + self.closer
                return
        else:
            assert False, "Head too long: {0}".format(self.head)

        line = inherits_base + ' :'

        for index, item in enumerate(self.inherits):
            item_and_sep = item + (',' if index < len(self.inherits) - 1 else self.closer)
            try_line = line + ' ' + item_and_sep

            if not self._line_fits(indent_level, try_line):
                yield line
                line = ' ' * (len(inherits_base) + 3) + item_and_sep
            else:
                line = try_line

        yield line
