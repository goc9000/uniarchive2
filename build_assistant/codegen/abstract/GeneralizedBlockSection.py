# build_assistant/codegen/abstract/GeneralizedBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.AbstractBlockSection import AbstractBlockSection
from build_assistant.codegen.abstract.GeneralizedHeadSection import GeneralizedHeadSection
from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection


class GeneralizedBlockSection(AbstractBlockSection):
    """
    A generalized block is a section of code that looks like this:

        HEAD(param1, param2, param3) : inherit1, inherit2, inherit3 {
          ...
        }

    or, alternatively, when items don't fit:

        HEAD(
          param1,
          param2
          param3
        ) : inherit1, inherit2,
            inherit3 {
          ...
        }

    Note how this matches functions, for loops, class definitions, etc. The fully general form is:

        <head> [ "(" <params[0]> <param_separator> params[1] <param_separator> ... ")" ] <decorations>
               [ ":" <inherits[0]> "," <inherits[1]> "," ... ] "{"
        <indented content>
        "}" [";"]
        [newline]
    """

    head = None
    params = None
    param_separator = None
    inherits = None
    decorations = None
    semicolon = None
    nl_after = None

    def __init__(
        self,
        source,
        head,
        params=None,
        param_separator=',',
        inherits=None,
        decorations=None,
        semicolon=False,
        nl_after=True
    ):
        super().__init__(source)

        self.head = head
        self.params = params
        self.param_separator = param_separator
        self.inherits = inherits
        self.decorations = decorations
        self.semicolon = semicolon
        self.nl_after = nl_after

    def _gen_header_items(self, indent_level):
        yield GeneralizedHeadSection(
            self.source,
            self.head,
            params=self.params,
            param_separator=self.param_separator,
            inherits=self.inherits,
            decorations=self.decorations,
            closer=' {'
        )

    def _gen_items(self, indent_level):
        content = IndentedCodeSection(self.source, 1)
        content.content_items = list(self._gen_content_items())
        yield content

    def _gen_footer_items(self, indent_level):
        yield '}' + (';' if self.semicolon else '')

        if self.nl_after:
            yield ''
