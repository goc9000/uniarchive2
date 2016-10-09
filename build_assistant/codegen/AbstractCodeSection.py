# build_assistant/codegen/AbstractCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.codegen_utils import filter_lines


class AbstractCodeSection:
    source = None

    def __init__(self, source):
        self.source = source

    def gen_lines(self):
        def gen():
            for item in self.gen_items():
                if isinstance(item, AbstractCodeSection):
                    for line in item.gen_lines():
                        yield line
                elif item == '':
                    yield ''
                else:
                    yield item

        return filter_lines(gen())

    def gen_items(self):
        raise NotImplementedError
