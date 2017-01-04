# build_assistant/codegen/AutoGenConfig.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.util.FrozenStruct import FrozenStruct


class ParamInfo(FrozenStruct):
    def __init__(self, _superclass_fields=None, **kwargs):
        FrozenStruct.__init__(
            self,
            ['type', 'name', 'unused'] + (_superclass_fields or list()),
            {'unused': False, **kwargs}
        )

    def render_for_declaration(self):
        return self.type + ' ' + self.name

    def render_for_implementation(self):
        return self.type + (' UNUSED ' if self.unused else ' ') + self.name


def parse_param_info(item):
    if isinstance(item, ParamInfo):
        return item

    type, name = item

    return ParamInfo(type=type, name=name)
