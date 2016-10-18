# build_assistant/codegen/AbstractCodeSectionWithContent.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re

from build_assistant.codegen.AbstractCodeSection import AbstractCodeSection
from build_assistant.autogen_common import BEGIN_CUSTOM_SECTION_LINE_PREFIX, END_CUSTOM_SECTION_LINE_PREFIX


class AbstractCodeSectionWithContent(AbstractCodeSection):
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

    # Indent

    def indented_section(self):
        from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection

        return self.subsection(IndentedCodeSection(self.source, 1))

    def unindented_section(self):
        from build_assistant.codegen.IndentedCodeSection import IndentedCodeSection

        return self.subsection(IndentedCodeSection(self.source, -1))

    # Toplevel blocks

    def enum_class_block(self, name):
        from build_assistant.codegen.EnumBlockSection import EnumBlockSection

        return self.subsection(EnumBlockSection(self.source, name, enum_class=True))

    def class_block(self, name, inherits=None):
        return self._struct_or_class_block(name, inherits, False)

    def struct_block(self, name, inherits=None):
        return self._struct_or_class_block(name, inherits, True)

    def _struct_or_class_block(self, name, inherits, struct):
        from build_assistant.codegen.ClassBlockSection import ClassBlockSection

        inherits = inherits or list()

        self.source.use_symbols(*inherits)

        return self.subsection(ClassBlockSection(self.source, name, inherits=inherits, struct=struct))

    def public_block(self):
        return self._colon_block('public')

    def private_block(self):
        return self._colon_block('private')

    def protected_block(self):
        return self._colon_block('protected')

    def _generalized_block(self, *args, **kwargs):
        from build_assistant.codegen.GeneralizedBlockSection import GeneralizedBlockSection

        return self.subsection(GeneralizedBlockSection(self.source, *args, **kwargs))

    def _generalized_head(self, *args, **kwargs):
        from build_assistant.codegen.GeneralizedHeadSection import GeneralizedHeadSection

        self.content_items.append(GeneralizedHeadSection(self.source, *args, **kwargs))

        return self

    def _colon_block(self, kind):
        from build_assistant.codegen.VisibilityBlockSection import VisibilityBlockSection

        return self.subsection(VisibilityBlockSection(self.source, kind))

    # Statement blocks

    def if_block(self, *conditions, operator='&&', nl_after=True):
        return self._generalized_block('if ', params=conditions, param_separator=' '+operator, nl_after=nl_after)

    def else_block(self):  # Note: use within if block
        with self.unindented_section() as section:
            section.line('} else {')

            return section.indented_section()

    def for_each_block(self, type, value, range, nl_after=True):
        self.source.use_symbol(type)

        return self._generalized_block('for ', params=['{0} {1} : {2}'.format(type, value, range)], nl_after=nl_after)

    def switch_block(self, switch_by):
        return self._generalized_block('switch ', params=[switch_by])

    def case_block(self, case_label):
        self.line('case ' + case_label + ':')

        return self.indented_section()

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
