# build_assistant/codegen/functions/MethodBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.GeneralizedBlockSection import GeneralizedBlockSection
from build_assistant.codegen.mixins.ProceduralCodeMixin import ProceduralCodeMixin


class MethodBlockSection(GeneralizedBlockSection, ProceduralCodeMixin):
    def __init__(self, source, class_name, function_name, return_type, *params, const=False):
        super().__init__(
            source,
            return_type + ' ' + class_name + '::' + function_name,
            params=[param.render_for_implementation() for param in params],
            decorations=(' const' if const else ''),
        )
