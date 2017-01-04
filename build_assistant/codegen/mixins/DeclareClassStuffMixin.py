# build_assistant/codegen/mixins/DeclareClassStuffMixin.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.mixins.NonProceduralCodeMixin import NonProceduralCodeMixin
from build_assistant.codegen.ParamInfo import parse_param_info


class DeclareClassStuffMixin(NonProceduralCodeMixin):
    def declare_constructor(self, class_name, *params):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        params = [parse_param_info(param) for param in params]

        assert isinstance(self, AbstractCodeSectionWithContent)

        self._use_symbols_for_params(params)

        return self._generalized_head(class_name, params=[param.render_for_declaration() for param in params])
