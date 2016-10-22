# build_assistant/codegen/statements/IfBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.statements.ElseBlockSection import ElseBlockSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin


class IfBlockSection(GeneralizedBlockSection, ProceduralCodeMixin):
    def __init__(self, source, conditions, operator='&&', nl_after=True):
        super().__init__(
            source,
            'if ',
            params=conditions,
            param_separator=' '+operator,
            nl_after=nl_after,
        )

    def else_block(self):
        return self.subsection(ElseBlockSection(self.source))
