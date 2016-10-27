# build_assistant/autogen_raw_events.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.AutoGenConfig import RawEventConfig, RawEventFieldConfig, EnumConfig, EnumValue
from build_assistant.GenericPolymorphicAugment import GenericPolymorphicAugment
from build_assistant.GenericPolymorphicFieldAugment import GenericPolymorphicFieldAugment
from build_assistant.util.grammar import camelcase_to_underscore
from build_assistant.util.VirtualPath import VirtualPath


BASE_EVENTS_PATH = VirtualPath(['intermediate_format', 'events'])
SUBTYPE_ENUM = 'RawEventSubType'


def autogen_raw_events_index(autogen_config):
    yield BASE_EVENTS_PATH, 'RawEvent'

    for path, name, _ in autogen_config.raw_events:
        yield BASE_EVENTS_PATH.append(path), 'Raw' + name + 'Event'

    return []


def autogen_raw_events_subtype_enum(autogen_config):
    return BASE_EVENTS_PATH, SUBTYPE_ENUM, EnumConfig(
        values=[
            EnumValue(
                text=name,
                constant=camelcase_to_underscore(name).upper(),
                int_value=None,
                comment=None,
            ) for _, name, _ in sorted(autogen_config.raw_events, key=lambda tup: tup[0].add(tup[1]).to_text())
        ],
        internal_comment=None,
    )


def gen_raw_events(autogen_config, autogen_core):
    base_event_config = BaseEventConfigAugment(autogen_config.base_raw_event, autogen_core)

    base_event_h = gen_base_raw_event(base_event_config, autogen_core)

    for rel_path, name, event_config in autogen_config.raw_events:
        event_config = EventConfigAugment(name, event_config, autogen_core, base_config=base_event_config)

        cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH.append(rel_path), event_config.class_name())
        h_source.cover_symbols_from(base_event_h)

        event_config.gen_code(cpp_source, h_source)


def gen_base_raw_event(base_event_config, autogen_core):
    cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH, base_event_config.class_name())

    base_event_config.gen_code(cpp_source, h_source)

    return h_source


class AbstractEventConfigAugment(GenericPolymorphicAugment):
    def __init__(self, event_config, autogen_core):
        assert isinstance(event_config, RawEventConfig), 'Augmented object should be RawEventConfig'

        GenericPolymorphicAugment.__init__(self, event_config, autogen_core, field_augment=EventFieldAugment)

    def gen_subtype_method(self, cpp_code, block):
        raise NotImplementedError

    def gen_event_name_method(self, cpp_code, block):
        raise NotImplementedError

    def gen_debug_write_details_method(self, cpp_code, block):
        raise NotImplementedError


class BaseEventConfigAugment(AbstractEventConfigAugment):
    def __init__(self, event_config, autogen_core):
        AbstractEventConfigAugment.__init__(self, event_config, autogen_core)

    def mandatory_base_fields(self):
        return list()

    def class_name(self):
        return 'RawEvent'

    def parent_class(self, no_template=None):
        return None

    def gen_code_impl(self, cpp_source, h_source, public_block, protected_block, private_block):
        with public_block as block:
            self.gen_field_declarations(block)
            self.gen_constructors(cpp_source.code, block)
            self.gen_mandatory_fields_sanity_check_method(cpp_source.code, private_block)

            block.nl().line('POLYMORPHIC_HELPERS').nl()
            block.source.include("utils/language/polymorphic_helpers.h")

            self.gen_subtype_method(cpp_source.code, block)
            block.nl()
            self.gen_event_name_method(cpp_source.code, block)
            block.nl()
            self.gen_debug_write_method(cpp_source.code, block)

        self.gen_debug_write_details_method(cpp_source.code, protected_block)

        self.gen_debug_write_operator(cpp_source.code, h_source.code)

    def gen_subtype_method(self, cpp_code, struct_block):
        struct_block.declare_fn('subType', SUBTYPE_ENUM, const=True, virtual=True, abstract=True)

    def gen_event_name_method(self, cpp_code, block):
        with cpp_code.method(
            self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=block
        ) as method:
            method.ret('name_for_raw_event_sub_type(subType())')

    def gen_debug_write_method(self, cpp_code, block):
        time_field = None
        index_field = None

        remaining_fields = []
        for field_config in self.fields:
            if field_config.name == 'timestamp' and time_field is None:
                time_field = field_config
            elif field_config.name.startswith('index') and index_field is None:
                index_field = field_config
            else:
                remaining_fields.append(field_config)

        with cpp_code.method(
            'RawEvent', 'writeToDebugStream', 'void', ('QDebug', 'stream'), const=True, declare_in=block
        ) as method:
            method \
                .declare_var('QDebugStateSaver', 'saver(stream)') \
                .code_line('stream.nospace()').nl() \
                .code_line('stream << "#" << {0} << " "', index_field.as_print_rvalue(cpp_code)) \
                .code_line('stream << "[" << {0} << "] "', time_field.as_print_rvalue(cpp_code)).nl() \
                .code_line('stream << QP(eventName())') \
                .code_line('writeDetailsToDebugStream(stream)').nl()
            cpp_code.source.use_symbol('QP')

            self.gen_debug_write_field_code(method, remaining_fields)

    def gen_debug_write_details_method(self, cpp_code, block):
        with cpp_code.method(
            self.class_name(), 'writeDetailsToDebugStream', 'void', ('QDebug UNUSED', 'stream'),
            const=True, virtual=True, declare_in=block
        ) as _:
            pass


class EventConfigAugment(AbstractEventConfigAugment):
    _name = None
    _base_config = None

    def __init__(self, name, event_config, autogen_core, base_config=None):
        AbstractEventConfigAugment.__init__(self, event_config, autogen_core)
        self._name = name
        self._base_config = base_config

    def mandatory_base_fields(self):
        return [f for f in self._base_config.fields if f.is_mandatory()]

    def class_name(self):
        return 'Raw' + self._name + 'Event'

    def parent_class(self, no_template=False):
        if self.fail_reason_enum is None:
            return 'RawEvent'
        elif no_template:
            return 'RawFailableEvent'
        else:
            return 'RawFailableEvent<{0}>'.format(self.fail_reason_enum)

    def gen_code_impl(self, cpp_source, h_source, public_block, protected_block, private_block):
        with public_block as block:
            self.gen_field_declarations(block)
            self.gen_constructors(cpp_source.code, block)
            self.gen_mandatory_fields_sanity_check_method(cpp_source.code, private_block)

            block.nl()
            self.gen_subtype_method(cpp_source.code, block)
            block.nl()
            self.gen_event_name_method(cpp_source.code, block)

        self.gen_debug_write_details_method(cpp_source.code, protected_block)

    def gen_subtype_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(), 'subType', SUBTYPE_ENUM, const=True, virtual=True, declare_in=struct_block
        ) as method:
            method.ret('{0}::{1}', SUBTYPE_ENUM, camelcase_to_underscore(self._name).upper())

    def gen_event_name_method(self, cpp_code, struct_block):
        if self.custom_name_method:
            with cpp_code.method(
                self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=struct_block
            ) as method:
                method.custom_section('Name method')

    def gen_debug_write_details_method(self, cpp_code, struct_block):
        with cpp_code.method(
            self.class_name(),
            'write' + ('FailableEvent' if self.fail_reason_enum is not None else '') + 'DetailsToDebugStream',
            'void',
            ('QDebug' + (' UNUSED' if len(self.fields) == 0 else ''), 'stream'),
            const=True, virtual=True, declare_in=struct_block
        ) as method:
            if self.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                self.gen_debug_write_field_code(method, self.fields)


class EventFieldAugment(GenericPolymorphicFieldAugment):
    def __init__(self, field_config, autogen_core):
        assert isinstance(field_config, RawEventFieldConfig), 'Augmented object should be RawEventFieldConfig'

        GenericPolymorphicFieldAugment.__init__(self, field_config, autogen_core)
