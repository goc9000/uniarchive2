# build_assistant/codegen/functions/ConstructorBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin


class ConstructorBlockSection(GeneralizedBlockSection, ProceduralCodeMixin):
    def __init__(self, source, class_name, *params, inherits=None):
        super().__init__(
            source,
            class_name + '::' + class_name,
            params=[type + ' ' + name for type, name in params],
            inherits=inherits,
        )
