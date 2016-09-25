# build_assistant/AutoGenConfig.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re

from enum import Enum
from collections import namedtuple

from build_assistant.VirtualPath import VirtualPath
from build_assistant.FrozenStruct import FrozenStruct


AutoGenConfig = namedtuple(
    'AutoGenConfig',
    ['enums', 'base_raw_event', 'raw_events', 'content_items'],
)
AutoGenEntry = namedtuple('AutoGenEntry', ['path', 'name', 'config'])

EnumConfig = namedtuple('EnumConfig', ['values', 'internal_comment'])
EnumValue = namedtuple('EnumValue', ['text', 'constant', 'int_value', 'comment'])

GenericEntityConfig = namedtuple('GenericEntityConfig', ['fields', 'field_breaks', 'options'])
GenericEntityFieldConfig = namedtuple('GenericEntityFieldConfig', ['expression', 'options'])


def parse_autogen_config(raw_config):
    return AutoGenConfig(
        enums=parse_hierarchy(raw_config['enums'], parse_enum_config),
        base_raw_event=parse_raw_event_config(raw_config['base raw event']),
        raw_events=parse_hierarchy(raw_config['raw events'], parse_raw_event_config),
        content_items=parse_hierarchy(raw_config['content items'], parse_content_item_config),
    )


def parse_hierarchy(config, leaf_parser):
    result = list()

    def visit_hierarchy_rec(config_view, base_path):
        for yaml_path, subconfig in config_view.items():
            path = VirtualPath.from_text(yaml_path)
            if subconfig is None:
                subconfig = dict()

            basename = path.basename()

            if basename[0].isupper():
                result.append(AutoGenEntry(
                    path=base_path.append(path.parent()),
                    name=basename,
                    config=leaf_parser(subconfig)
                ))
            else:
                visit_hierarchy_rec(subconfig, base_path.append(path))

    visit_hierarchy_rec(config, VirtualPath([]))

    return result


def preparse_entity(entity_config, field_name):
    raw_fields = list()
    field_breaks = list()
    raw_options = dict()

    for subconfig in entity_config or list():
        if subconfig == '..':
            field_breaks.append(len(raw_fields))
        elif isinstance(subconfig, str):
            raw_fields.append(GenericEntityFieldConfig(expression=subconfig, options=dict()))
        elif field_name in subconfig:
            raw_fields.append(
                GenericEntityFieldConfig(
                    expression=subconfig[field_name],
                    options={k: v for k, v in subconfig.items() if k != field_name}
                )
            )
        else:
            raw_options.update(**subconfig)

    return GenericEntityConfig(fields=raw_fields, field_breaks=field_breaks, options=raw_options)


def parse_enum_config(entity_config):
    def text_to_constant_name(text):
        return '_'.join(word.upper() for word in re.findall('[a-z0-9]+', text, flags=re.IGNORECASE))

    def parse_enum_value(preparsed_field):
        match = re.match(r'^(.*?)(?:\s+as\s+([A-Z_]+))?(?:\s*=\s*(\d+))?(?:\s*//\s*(.*))?$', preparsed_field.expression)
        assert match is not None, "Invalid enum value expression: " + preparsed_field.expression

        text, constant, int_value, comment = match.groups()

        return EnumValue(
            text=text,
            constant=constant or text_to_constant_name(text),
            int_value=int_value,
            comment=comment
        )

    preparsed = preparse_entity(entity_config, 'value')

    return EnumConfig(
        values=[parse_enum_value(field) for field in preparsed.fields],
        internal_comment=preparsed.options.get('internal comment')
    )


def parse_generic_polymorphic_config(preparsed_entity, field_parser, supported_prefixes=None):
    def parse_polymorphic_field(field_config):
        if supported_prefixes is not None:
            prefixes_pattern = r'((?:(?:' + r'|'.join(supported_prefixes) + r')\s+)*)'
        else:
            prefixes_pattern = r'()'

        match = re.match(
            r'^' + prefixes_pattern + r'(\?|\(\?\))?([a-zA-Z0-9_]+)(\[\]|\(\[\]\))?\s+([a-zA-Z0-9_]+)' +
            r'(?:\s+as\s+([a-zA-Z0-9_]+))?(?:\s*=\s*(\w+))?$',
            field_config.expression.strip()
        )
        assert match is not None, "Invalid field config: " + field_config.expression

        prefixes, optionality, base_type, multiplicity, name, short_name, default_value = match.groups()

        raw_attrs = dict(
            name=name,
            base_type=base_type,
            is_optional=(optionality is not None),
            is_list=(multiplicity is not None),
            add_to_constructor=(optionality == '(?)'),
            maybe_singleton=(multiplicity == '([])'),
            short_name=short_name,
            default_value=default_value,
            doc=field_config.options['doc'].strip() if 'doc' in field_config.options else None,
        )

        if supported_prefixes is not None:
            raw_attrs['prefixes'] = [item for item in re.split(r'\s+', prefixes) if item != '']

        return field_parser(raw_attrs)

    return dict(
        fields=[parse_polymorphic_field(preparsed) for preparsed in preparsed_entity.fields],
        field_breaks=preparsed_entity.field_breaks,
    )


def parse_raw_event_config(entity_config):
    def parse_raw_event_field(attrs):
        return RawEventFieldConfig(**attrs)

    preparsed = preparse_entity(entity_config, 'field')

    custom = preparsed.options.get('custom', list())
    assert set(custom).issubset({
        'name method', 'debug write method'
    }), 'Unsupported custom directive'

    return RawEventConfig(
        **parse_generic_polymorphic_config(preparsed, parse_raw_event_field),
        fail_reason_enum=preparsed.options.get('failable'),
        custom_name_method='name method' in custom,
        custom_debug_write_method='debug write method' in custom,
    )


def parse_content_item_config(entity_config):
    def parse_content_item_field(attrs):
        return ContentItemFieldConfig(**attrs)

    def parse_content_item_tag_field(attrs):
        prefixes = attrs['prefixes']
        del attrs['prefixes']
        return ContentItemTagFieldConfig(**attrs, is_primary=('primary' in prefixes))

    def parse_tag_type(tag_type):
        if tag_type is None:
            return None
        elif tag_type == 'standard':
            return ContentItemTagType.STANDARD
        elif tag_type == 'symmetric':
            return ContentItemTagType.SYMMETRIC
        elif tag_type == 'self-closing':
            return ContentItemTagType.SELF_CLOSING

        assert False, 'Unsupported tag type: {0}'.format(tag_type)

    preparsed = preparse_entity(entity_config, 'field')

    custom = preparsed.options.get('custom', list())
    assert set(custom).issubset({
        'debug write method'
    }), 'Unsupported custom directive'

    if 'tag type' in preparsed.options:
        return ContentItemTagConfig(
            **parse_generic_polymorphic_config(preparsed, parse_content_item_tag_field, supported_prefixes=['primary']),
            tag_type=parse_tag_type(preparsed.options['tag type']),
            tag_name_override=preparsed.options.get('tag name'),
            custom_debug_write_method=('debug write method' in custom),
        )
    else:
        return ContentItemConfig(
            **parse_generic_polymorphic_config(preparsed, parse_content_item_field),
            custom_debug_write_method=('debug write method' in custom),
        )


class GenericPolymorphicConfig(FrozenStruct):
    def __init__(self, _superclass_fields=None, **kwargs):
        FrozenStruct.__init__(
            self,
            ['fields', 'field_breaks'] + (_superclass_fields or list()),
            kwargs
        )


class GenericPolymorphicFieldConfig(FrozenStruct):
    def __init__(self, _superclass_fields=None, **kwargs):
        FrozenStruct.__init__(
            self,
            ['name', 'base_type', 'is_optional', 'is_list', 'add_to_constructor', 'maybe_singleton', 'short_name',
             'default_value', 'doc'] + (_superclass_fields or list()),
            kwargs
        )


class RawEventConfig(GenericPolymorphicConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        GenericPolymorphicConfig.__init__(
            self,
            _superclass_fields=['fail_reason_enum', 'custom_name_method', 'custom_debug_write_method'] +
                (_superclass_fields or list()),
            **kwargs
        )


class RawEventFieldConfig(GenericPolymorphicFieldConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        GenericPolymorphicFieldConfig.__init__(
            self,
            _superclass_fields=[] + (_superclass_fields or list()),
            **kwargs
        )


class ContentItemConfig(GenericPolymorphicConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        GenericPolymorphicConfig.__init__(
            self,
            _superclass_fields=['custom_debug_write_method'] + (_superclass_fields or list()),
            **kwargs
        )


class ContentItemFieldConfig(GenericPolymorphicFieldConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        GenericPolymorphicFieldConfig.__init__(
            self,
            _superclass_fields=[] + (_superclass_fields or list()),
            **kwargs
        )


ContentItemTagType = Enum('ContentItemTagType', ['STANDARD', 'SYMMETRIC', 'SELF_CLOSING'])


class ContentItemTagConfig(ContentItemConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        ContentItemConfig.__init__(
            self,
            _superclass_fields=['tag_type', 'tag_name_override'] + (_superclass_fields or list()),
            **kwargs
        )


class ContentItemTagFieldConfig(ContentItemFieldConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        ContentItemFieldConfig.__init__(
            self,
            _superclass_fields=['is_primary'] + (_superclass_fields or list()),
            **kwargs
        )
