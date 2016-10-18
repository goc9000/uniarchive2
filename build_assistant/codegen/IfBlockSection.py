# build_assistant/codegen/IfBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.ElseBlockSection import ElseBlockSection


class IfBlockSection(GeneralizedBlockSection):
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
