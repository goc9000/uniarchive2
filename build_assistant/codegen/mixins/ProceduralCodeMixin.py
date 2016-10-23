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
        from build_assistant.codegen.statements.SwitchBlockSection import SwitchBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.subsection(SwitchBlockSection(self.source, switch_by))
