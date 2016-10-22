# build_assistant/codegen/statements/SwitchBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.statements.CaseBlockSection import CaseBlockSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin


class SwitchBlockSection(GeneralizedBlockSection, ProceduralCodeMixin):
    def __init__(self, source, switch_by, nl_after=True):
        super().__init__(source, 'switch ', params=[switch_by], nl_after=nl_after)

    def case_block(self, case_label):
        return self.subsection(CaseBlockSection(self.source, case_label))
