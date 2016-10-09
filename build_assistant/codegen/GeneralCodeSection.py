# build_assistant/codegen/GeneralCodeSection.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.codegen.AbstractCodeSectionWithContent import AbstractCodeSectionWithContent


class GeneralCodeSection(AbstractCodeSectionWithContent):

    def __init__(self, source):
        super().__init__(source)

    def gen_items(self, indent_level):
        return self.gen_content_items()
