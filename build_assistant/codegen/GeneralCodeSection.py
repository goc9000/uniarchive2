# build_assistant/codegen/GeneralCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent


class GeneralCodeSection(AbstractCodeSectionWithContent):

    def __init__(self, source, initial_indent_level):
        super().__init__(source, initial_indent_level)

    def gen_items(self):
        return self.gen_content_items()
