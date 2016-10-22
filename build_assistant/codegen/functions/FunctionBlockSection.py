# build_assistant/codegen/functions/FunctionBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin


class FunctionBlockSection(GeneralizedBlockSection, ProceduralCodeMixin):
    def __init__(self, source, function_name, return_type, *params):
        super().__init__(
            source,
            return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params],
        )
