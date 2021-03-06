# build_assistant/codegen/declarations/EnumBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.abstract.GeneralizedBlockSection import GeneralizedBlockSection


class EnumBlockSection(GeneralizedBlockSection):
    def __init__(self, source, name, enum_class=False, underlying_type=None):
        super().__init__(
            source,
            'enum ' + ('class ' if enum_class else '') + name,
            semicolon=True,
            inherits=[underlying_type] if underlying_type is not None else None
        )
