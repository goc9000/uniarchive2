# build_assistant/codegen/mixins/DeclareClassStuffMixin.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.mixins.NonProceduralCodeMixin import NonProceduralCodeMixin


class DeclareClassStuffMixin(NonProceduralCodeMixin):
    def declare_constructor(self, class_name, *params):
        from build_assistant.codegen.abstract.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent

        assert isinstance(self, AbstractCodeSectionWithContent)

        self.source.use_symbols(*(type for type, _ in params))

        return self._generalized_head(class_name, params=[type + ' ' + name for type, name in params])
