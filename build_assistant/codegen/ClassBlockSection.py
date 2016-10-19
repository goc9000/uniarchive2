# build_assistant/codegen/ClassBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.VisibilityBlockSection import VisibilityBlockSection


class ClassBlockSection(GeneralizedBlockSection):
    def __init__(self, source, name, inherits=None, struct=False):
        super().__init__(
            source,
            ('struct' if struct else 'class') + ' ' + name,
            inherits=inherits,
            semicolon=True,
        )

    def public_block(self):
        return self._visibility_block('public')

    def private_block(self):
        return self._visibility_block('private')

    def protected_block(self):
        return self._visibility_block('protected')

    def _visibility_block(self, kind):
        return self.subsection(VisibilityBlockSection(self.source, kind))
