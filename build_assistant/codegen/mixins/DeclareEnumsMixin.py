# build_assistant/codegen/mixins/DeclareEnumsMixin.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent


class DeclareEnumsMixin:
    def enum_class_block(self, name):
        from build_assistant.codegen.EnumBlockSection import EnumBlockSection

        assert isinstance(self, AbstractCodeSectionWithContent)

        return self.subsection(EnumBlockSection(self.source, name, enum_class=True))
