# build_assistant/codegen/AbstractCodeSectionWithContent.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re

from contextlib import contextmanager

from build_assistant.codegen.AbstractCodeSection import AbstractCodeSection
from build_assistant.codegen.GeneralizedHeadSection import GeneralizedHeadSection
from build_assistant.autogen_common import BEGIN_CUSTOM_SECTION_LINE_PREFIX, END_CUSTOM_SECTION_LINE_PREFIX


class AbstractCodeSectionWithContent(AbstractCodeSection):
    content_items = None
    indent_level = None

    def __init__(self, source, initial_indent_level):
        super().__init__(source)

        self.content_items = list()
        self.indent_level = initial_indent_level

    def gen_content_items(self):
        for item in self.content_items:
            yield item

    def deepest_open_section(self):
        if len(self.content_items) == 0:
            return self

        if not isinstance(self.content_items[-1], AbstractCodeSectionWithContent):
            return self

        return self.content_items[-1].deepest_open_section()

    # Basics

    def line(self, line):
        self.content_items.append((self._get_indent() + line).rstrip())
        return self

    def nl(self):
        return self.line('')

    def subsection(self, section):
        self.content_items.append(section)
        return self

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

    @contextmanager
    def indented_section(self):
        from build_assistant.codegen.GeneralCodeSection import GeneralCodeSection

        section = GeneralCodeSection(self.source, self.indent_level + 1)

        self.content_items.append(section)

        yield section

    @contextmanager
    def unindented_section(self):
        from build_assistant.codegen.GeneralCodeSection import GeneralCodeSection

        section = GeneralCodeSection(self.source, self.indent_level - 1)

        self.content_items.append(section)

        yield section

    def line_fits(self, line):
        return self.indent_level * self.source.core.codegen_cfg.indent_size + len(line) \
               <= self.source.core.codegen_cfg.gutter_width

    def _get_indent(self):
        return ' ' * (self.indent_level * self.source.core.codegen_cfg.indent_size)

    # Toplevel blocks

    def enum_class_block(self, name):
        return self._generalized_block('enum class ' + name, semicolon=True)

    def struct_block(self, name, inherits=None):
        inherits = inherits or list()

        self.source.use_symbols(*inherits)

        return self._generalized_block('struct ' + name, inherits=inherits, semicolon=True)

    def public_block(self):
        return self._colon_block('public')

    def private_block(self):
        return self._colon_block('private')

    def protected_block(self):
        return self._colon_block('protected')

    @contextmanager
    def _generalized_block(
        self,
        head,
        params=None,
        param_separator=',',
        inherits=None,
        decorations=None,
        semicolon=False,
        nl_after=True
    ):
        """
        A generalized block looks like this:
        HEAD(param1, param2, param3)[decorations] : inherit1, inherit2, inherit3 {
        }

        or, alternatively, when items don't fit:

        HEAD(
          param1,
          param2
          param3
        )[decorations] : inherit1, inherit2,
                         inherit3 {
        }
        """

        self._generalized_head(
            head,
            params=params,
            param_separator=param_separator,
            inherits=inherits,
            decorations=decorations,
            closer=' {'
        )

        with self.indented_section() as section:
            yield section

        self.line('}' + (';' if semicolon else ''))

        if nl_after:
            self.nl()

        return self

    def _generalized_head(self, *args, **kwargs):
        self.content_items.append(GeneralizedHeadSection(self.source, self.indent_level, *args, **kwargs))

        return self

    @contextmanager
    def _colon_block(self, kind):
        with self.unindented_section() as section:
            section.line(kind + ':')

            with section.indented_section() as content_section:
                yield content_section

    # Statement blocks

    def if_block(self, *conditions, operator='&&', nl_after=True):
        return self._generalized_block('if ', params=conditions, param_separator=' '+operator, nl_after=nl_after)

    @contextmanager
    def else_block(self):  # Note: use within if block
        with self.unindented_section() as section:
            section.line('} else {')

            with section.indented_section() as content_section:
                yield content_section

    def for_each_block(self, type, value, range, nl_after=True):
        self.source.use_symbol(type)

        return self._generalized_block('for ', params=['{0} {1} : {2}'.format(type, value, range)], nl_after=nl_after)

    def switch_block(self, switch_by):
        return self._generalized_block('switch ', params=[switch_by])

    @contextmanager
    def case_block(self, case_label):
        self.line('case ' + case_label + ':')

        with self.indented_section() as section:
            yield section

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

    def function(self, function_name, return_type, *params, declare=False):
        if declare:
            self._companion_code().declare_fn(function_name, return_type, *_adjust_params_for_declare(params))

        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self._generalized_block(
            return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params]
        )

    def method(self, class_name, function_name, return_type, *params, const=False, virtual=False, declare=False):
        if declare:
            self._companion_code().declare_fn(
                function_name, return_type, *_adjust_params_for_declare(params), const=const, virtual=virtual
            )

        self.source.use_symbols(return_type, *(type for type, _ in params))

        return self._generalized_block(
            return_type + ' ' + class_name + '::' + function_name,
            params=[type + ' ' + name for type, name in params],
            decorations=(' const' if const else '')
        )

    def constructor(self, class_name, *params, inherits=None, declare=False):
        if declare:
            self._companion_code().declare_constructor(class_name, *_adjust_params_for_declare(params))

        self.source.use_symbols(*(type for type, _ in params))

        return self._generalized_block(
            class_name + '::' + class_name,
            params=[type + ' ' + name for type, name in params],
            inherits=inherits
        )

    def _companion_code(self):
        return self.source.companion.code.deepest_open_section()

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
