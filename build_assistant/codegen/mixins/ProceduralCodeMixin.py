# build_assistant/codegen/mixins/ProceduralCodeMixin.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


class ProceduralCodeMixin:
    """
    This mixin is added to code sections that contain procedural constructs, such as if, else, switch, etc. Examples
    of such sections include functions, methods, as well as if/for/etc. blocks themselves.
    """
    def code_line(self, line_format, *args, **kwargs):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.line(line_format.format(*args, **kwargs) + ';')

    def ret(self, expr_format, *args, **kwargs):
        """
        Shortcut for the frequently used "return ...;" code line.
        """
        return self.code_line('return ' + expr_format, *args, **kwargs)

    def declare_var(self, type, name, default_value=None):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbol(type)

        return self.line('{0} {1}{2};'.format(type, name, ' = ' + default_value if default_value is not None else ''))

    def call(self, function, *values):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self._generalized_head(function, params=values)

    def if_block(self, *conditions, operator='&&', nl_after=True):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.statements.IfBlockSection import IfBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.subsection(IfBlockSection(self.source, conditions, operator=operator, nl_after=nl_after))

    def for_each_block(self, type, value, range, nl_after=True):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.statements.ForEachBlockSection import ForEachBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbol(type)

        return self.subsection(ForEachBlockSection(self.source, type, value, range, nl_after=nl_after))

    def switch_block(self, switch_by):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
        from build_assistant.codegen.statements.switch.SwitchBlockSection import SwitchBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.subsection(SwitchBlockSection(self.source, switch_by))
