# build_assistant/autogen/content_items/TagContentItemFieldCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.AutoGenConfig import TagContentItemType, TagContentItemFieldConfig
from build_assistant.autogen.content_items.ContentItemFieldCodeGenerator import ContentItemFieldCodeGenerator


class TagContentItemFieldCodeGenerator(ContentItemFieldCodeGenerator):
    _parent_tag = None

    def __init__(self, field_config, autogen_core, parent_tag):
        assert isinstance(field_config, TagContentItemFieldConfig), \
            'Augmented object should be TagContentItemFieldConfig'

        ContentItemFieldCodeGenerator.__init__(self, field_config, autogen_core)

        self._parent_tag = parent_tag

    def _extra_clone_params(self):
        return dict(parent_tag=self._parent_tag)

    def _is_optional_for_storage(self):
        return self.is_optional or self._parent_tag.tag_type == TagContentItemType.STANDARD

    def as_serialize_rvalue(self, destination_code):
        rvalue = super().as_serialize_rvalue(destination_code)

        if self._parent_tag.tag_type == TagContentItemType.STANDARD and self.uses_optional() and not self.is_optional:
            rvalue = '*' + rvalue

        return rvalue

    def debug_write_header(self):
        if self.is_primary:
            return '" "'

        return super().debug_write_header()
