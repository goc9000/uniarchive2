# build_assistant/autogen/content_items/ContentItemFieldCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.AutoGenConfig import ContentItemFieldConfig
from build_assistant.autogen.GenericPolymorphicFieldCodeGenerator import GenericPolymorphicFieldCodeGenerator


class ContentItemFieldCodeGenerator(GenericPolymorphicFieldCodeGenerator):
    def __init__(self, field_config, autogen_core):
        assert isinstance(field_config, ContentItemFieldConfig), 'Augmented object should be ContentItemFieldConfig'

        GenericPolymorphicFieldCodeGenerator.__init__(self, field_config, autogen_core)
