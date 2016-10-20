# build_assistant/codegen/ForEachBlockSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.GeneralizedBlockSection import GeneralizedBlockSection


class ForEachBlockSection(GeneralizedBlockSection):
    def __init__(self, source, type, value, range, nl_after=True):
        super().__init__(
            source,
            'for ',
            params=['{0} {1} : {2}'.format(type, value, range)],
            nl_after=nl_after,
        )
