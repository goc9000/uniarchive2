# build_assistant/codegen/AbstractCodeSectionWithContent.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re

from build_assistant.codegen.AbstractCodeSection import AbstractCodeSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin
from build_assistant.autogen_common import BEGIN_CUSTOM_SECTION_LINE_PREFIX, END_CUSTOM_SECTION_LINE_PREFIX


class AbstractCodeSectionWithContent(AbstractCodeSection, ProceduralCodeMixin):
    content_items = None

    def __init__(self, source):
        super().__init__(source)

        self.content_items = list()

    def _gen_content_items(self):
        for item in self.content_items:
            yield item

    # Basics

    def line(self, line):
        self.content_items.append(line.rstrip())
        return self

    def nl(self):
        return self.line('')

    def subsection(self, section):
        self.content_items.append(section)
        return section

    # Comments

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

    # Toplevel blocks

    def _generalized_block(self, *args, **kwargs):
        from build_assistant.codegen.GeneralizedBlockSection import GeneralizedBlockSection

        return self.subsection(GeneralizedBlockSection(self.source, *args, **kwargs))

    def _generalized_head(self, *args, **kwargs):
        from build_assistant.codegen.GeneralizedHeadSection import GeneralizedHeadSection

        self.content_items.append(GeneralizedHeadSection(self.source, *args, **kwargs))

        return self

    # Functions

    def declare_fn(self, function_name, return_type, *params, const=False, virtual=False, abstract=False):
        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self._generalized_head(
            ('virtual ' if virtual else '') + return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params],
            decorations=(' const' if const else '') + (' = 0' if abstract else '')
        )

    def declare_constructor(self, class_name, *params):
        self.source.use_symbols(*(type for type, _ in params))

        return self._generalized_head(class_name, params=[type + ' ' + name for type, name in params])

    def function(self, function_name, return_type, *params, declare_in=None):
        if declare_in is not None:
            declare_in.declare_fn(function_name, return_type, *_adjust_params_for_declare(params))

        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self._generalized_block(
            return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params]
        )

    def method(self, class_name, function_name, return_type, *params, const=False, virtual=False, declare_in=None):
        if declare_in is not None:
            declare_in.declare_fn(
                function_name, return_type, *_adjust_params_for_declare(params), const=const, virtual=virtual
            )

        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self._generalized_block(
            return_type + ' ' + class_name + '::' + function_name,
            params=[type + ' ' + name for type, name in params],
            decorations=(' const' if const else '')
        )

    def constructor(self, class_name, *params, inherits=None, declare_in=None):
        if declare_in is not None:
            declare_in.declare_constructor(class_name, *_adjust_params_for_declare(params))

        self.source.use_symbols(*(type for type, _ in params))

        return self._generalized_block(
            class_name + '::' + class_name,
            params=[type + ' ' + name for type, name in params],
            inherits=inherits
        )

    # Fields

    def field(self, type, name, default_value=None):
        self.source.use_symbol(type)

        return self.line('{0} {1}{2};'.format(type, name, ' = ' + default_value if default_value is not None else ''))

    # Calls

    def call(self, function, *values):
        return self._generalized_head(function, params=values)

    # Custom sections

    def custom_section(self, name):
        self.source.custom_sections.add(name)

        self.line(BEGIN_CUSTOM_SECTION_LINE_PREFIX + name)
        self.line(END_CUSTOM_SECTION_LINE_PREFIX + name)

        return self


def _adjust_params_for_declare(params):
    return [(re.sub(r'\s+UNUSED\s*$', '', type), name) for type, name in params]
