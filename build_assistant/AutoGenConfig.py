# build_assistant/AutoGenConfig.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re

from collections import namedtuple

from build_assistant.VirtualPath import VirtualPath


AutoGenConfig = namedtuple('AutoGenConfig', ['enums', 'base_raw_event', 'raw_events'])
AutoGenEntry = namedtuple('AutoGenEntry', ['path', 'name', 'config'])

EnumConfig = namedtuple('EnumConfig', ['values', 'internal_comment'])
EnumValue = namedtuple('EnumValue', ['text', 'constant', 'int_value', 'comment'])

RawEventConfig = namedtuple('RawEventConfig', ['fields', 'field_breaks', 'fail_reason_enum'])
RawEventFieldConfig = namedtuple(
    'RawEventConfig',
    ['name', 'base_type', 'is_optional', 'is_list', 'add_to_constructor', 'maybe_singleton', 'short_name',
     'default_value', 'doc']
)

GenericEntityConfig = namedtuple('GenericEntityConfig', ['fields', 'field_breaks', 'options'])
GenericEntityFieldConfig = namedtuple('GenericEntityFieldConfig', ['expression', 'options'])


def parse_autogen_config(raw_config):
    return AutoGenConfig(
        enums=parse_hierarchy(raw_config['enums'], parse_enum_config),
        base_raw_event=parse_raw_event_config(raw_config['base raw event']),
        raw_events=parse_hierarchy(raw_config['raw events'], parse_raw_event_config),
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

    for subconfig in entity_config:
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


def parse_raw_event_config(entity_config):
    def parse_raw_event_field(field_config):
        match = re.match(
            r'^(\?|\(\?\))?([a-zA-Z0-9_]+)(\[\]|\(\[\]\))?\s+([a-zA-Z0-9_]+)' +
            r'(?:\s+as\s+([a-zA-Z0-9_]+))?(?:\s*=\s*(\w+))?$',
            field_config.expression.strip()
        )
        assert match is not None, "Invalid field config: " + field_config.expression

        optionality, base_type, multiplicity, name, short_name, default_value = match.groups()

        return RawEventFieldConfig(
            name=name,
            base_type=base_type,
            is_optional=(optionality is not None),
            is_list=(multiplicity is not None),
            add_to_constructor=(optionality == '(?)'),
            maybe_singleton=(multiplicity == '([])'),
            short_name=short_name,
            default_value=default_value,
            doc=field_config.options['doc'].strip() if 'doc' in field_config.options else None
        )

    preparsed = preparse_entity(entity_config, 'field')

    return RawEventConfig(
        fields=[parse_raw_event_field(preparsed) for preparsed in preparsed.fields],
        field_breaks=preparsed.field_breaks,
        fail_reason_enum=preparsed.options.get('failable')
    )
