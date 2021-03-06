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

from build_assistant.util.grammar import camelcase_to_underscore, text_to_constant_name
from build_assistant.util.FrozenStruct import FrozenStruct
from build_assistant.util.VirtualPath import VirtualPath


AutoGenConfig = namedtuple(
    'AutoGenConfig',
    ['enums', 'base_raw_event', 'raw_events', 'content_items'],
)
AutoGenEntry = namedtuple('AutoGenEntry', ['path', 'name', 'config'])

EnumConfig = namedtuple('EnumConfig', ['values', 'internal_comment', 'underlying_type_override'])
EnumValue = namedtuple('EnumValue', ['text', 'constant', 'int_value', 'comment'])

GenericEntityConfig = namedtuple('GenericEntityConfig', ['fields', 'field_breaks', 'options'])
GenericEntityFieldConfig = namedtuple('GenericEntityFieldConfig', ['expression', 'options'])


def parse_autogen_config(raw_config):
    return AutoGenConfig(
        enums=parse_hierarchy(raw_config['enums'], parse_enum_config) +\
              parse_hierarchy(raw_config['poly subtype enums'], parse_poly_subtype_enum_config),
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
    def parse_enum_value(preparsed_field):
        match = re.match(r'^(.*?)(?:\s+as\s+([A-Z_]+))?(?:\s*=\s*(\d+))?(?:\s*//\s*(.*))?$', preparsed_field.expression)
        assert match is not None, "Invalid enum value expression: " + preparsed_field.expression

        text, constant, int_value, comment = match.groups()

        assert int_value is None or re.match(r'^-?[0-9]+$', int_value), \
            "Invalid enum value int_value (must be an int): " + int_value

        return EnumValue(
            text=text,
            constant=constant or text_to_constant_name(text),
            int_value=int(int_value) if int_value is not None else None,
            comment=comment,
        )

    preparsed = preparse_entity(entity_config, 'value')

    return EnumConfig(
        values=[parse_enum_value(field) for field in preparsed.fields],
        internal_comment=preparsed.options.get('internal comment'),
        underlying_type_override=preparsed.options.get('underlying type'),
    )


def parse_poly_subtype_enum_config(entity_config):
    return EnumConfig(
        values=[
            EnumValue(
                text=item,
                constant=camelcase_to_underscore(item).upper(),
                int_value=None,
                comment=None,
            ) for item in parse_vectorish(entity_config)
        ],
        internal_comment=None,
        underlying_type_override=None,
    )


def parse_vectorish(value):
    if value is None:
        return list()
    if isinstance(value, str):
        return [item.strip() for item in value.split(',') if item.strip() != ""]

    assert isinstance(value, list) or isinstance(value, tuple)

    return value


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

        assert base_type not in ['uint', 'int'], "Use ints of specified size for field '{0}'".format(name)

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
        return TagContentItemFieldConfig(**attrs, is_primary=('primary' in prefixes))

    def parse_tag_type(tag_type):
        if tag_type is None:
            return None
        elif tag_type == 'standard':
            return TagContentItemType.STANDARD
        elif tag_type == 'symmetric':
            return TagContentItemType.SYMMETRIC
        elif tag_type == 'self-closing':
            return TagContentItemType.SELF_CLOSING

        assert False, 'Unsupported tag type: {0}'.format(tag_type)

    preparsed = preparse_entity(entity_config, 'field')

    custom = preparsed.options.get('custom', list())
    assert set(custom).issubset({
        'debug write method'
    }), 'Unsupported custom directive'

    if 'tag type' in preparsed.options:
        return TagContentItemConfig(
            **parse_generic_polymorphic_config(preparsed, parse_content_item_tag_field, supported_prefixes=['primary']),
            tag_type=parse_tag_type(preparsed.options['tag type']),
            tag_name_override=preparsed.options.get('tag name'),
            debug_write_name_override=None,
            custom_debug_write_method=('debug write method' in custom),
        )
    else:
        return ContentItemConfig(
            **parse_generic_polymorphic_config(preparsed, parse_content_item_field),
            debug_write_name_override=preparsed.options.get('debug write name'),
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
            _superclass_fields=[
                'custom_debug_write_method', 'debug_write_name_override',
            ] + (_superclass_fields or list()),
            **kwargs
        )


class ContentItemFieldConfig(GenericPolymorphicFieldConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        GenericPolymorphicFieldConfig.__init__(
            self,
            _superclass_fields=[] + (_superclass_fields or list()),
            **kwargs
        )


TagContentItemType = Enum('ContentItemTagType', ['STANDARD', 'SYMMETRIC', 'SELF_CLOSING'])


class TagContentItemConfig(ContentItemConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        ContentItemConfig.__init__(
            self,
            _superclass_fields=['tag_type', 'tag_name_override'] + (_superclass_fields or list()),
            **kwargs
        )


class TagContentItemFieldConfig(ContentItemFieldConfig):
    def __init__(self, _superclass_fields=None, **kwargs):
        ContentItemFieldConfig.__init__(
            self,
            _superclass_fields=['is_primary'] + (_superclass_fields or list()),
            **kwargs
        )
