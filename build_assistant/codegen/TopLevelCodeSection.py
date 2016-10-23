# build_assistant/codegen/TopLevelCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent
from build_assistant.codegen.mixins.NonProceduralCodeMixin import NonProceduralCodeMixin


class TopLevelCodeSection(AbstractCodeSectionWithContent, NonProceduralCodeMixin):

    def __init__(self, source):
        super().__init__(source)

    def class_block(self, name, inherits=None):
        return self._struct_or_class_block(name, inherits, False)

    def struct_block(self, name, inherits=None):
        return self._struct_or_class_block(name, inherits, True)

    def _struct_or_class_block(self, name, inherits, struct):
        from build_assistant.codegen.declarations.ClassBlockSection import ClassBlockSection

        inherits = inherits or list()

        self.source.use_symbols(*inherits)

        return self.subsection(ClassBlockSection(self.source, name, inherits=inherits, struct=struct))

    def _gen_items(self, indent_level):
        return self._gen_content_items()
