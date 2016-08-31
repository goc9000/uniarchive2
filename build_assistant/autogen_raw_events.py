# build_assistant/autogen_raw_events.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.VirtualPath import VirtualPath
from build_assistant.AutoGenConfig import RawEventConfig, RawEventFieldConfig
from build_assistant.GenericPolymorphicAugment import GenericPolymorphicAugment
from build_assistant.GenericPolymorphicFieldAugment import GenericPolymorphicFieldAugment


BASE_EVENTS_PATH = VirtualPath(['intermediate_format', 'events'])


def autogen_raw_events_index(autogen_config):
    yield BASE_EVENTS_PATH, 'RawEvent'

    for path, name, _ in autogen_config.raw_events:
        yield BASE_EVENTS_PATH.append(path), 'Raw' + name + 'Event'

    return []


def gen_raw_events(autogen_config, autogen_core):
    base_event_config = BaseEventConfigAugment(autogen_config.base_raw_event, autogen_core)

    base_event_h = gen_base_raw_event(base_event_config, autogen_core)

    for rel_path, name, event_config in autogen_config.raw_events:
        event_config = EventConfigAugment(name, event_config, autogen_core, base_config=base_event_config)
        class_name = event_config.class_name()

        cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH.append(rel_path), class_name)

        with h_source.struct_block(class_name, inherits=[event_config.parent_class()]) as struct:
            h_source.cover_symbols_from(base_event_h)

            with struct.public_block() as block:
                event_config.gen_field_declarations(block)
                event_config.gen_constructors(cpp_source)
                event_config.gen_mandatory_fields_sanity_check_method(cpp_source)

                block.nl()

                with cpp_source.method(
                    class_name, 'eventName', 'QString', const=True, virtual=True, declare=True
                ) as method:
                    if event_config.custom_name_method:
                        method.custom_section('Name method')
                    else:
                        method.line("return {0};".format(method.string_literal(name)))

            struct.nl()

            with struct.protected_block() as _:
                gen_debug_write_method(cpp_source, class_name, event_config)

            event_config.gen_private_block(struct)


def gen_base_raw_event(base_event_config, autogen_core):
    class_name = base_event_config.class_name()

    cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH, class_name)

    with h_source.struct_block(class_name) as struct:
        with struct.public_block() as block:
            base_event_config.gen_field_declarations(block)
            base_event_config.gen_constructors(cpp_source)
            base_event_config.gen_mandatory_fields_sanity_check_method(cpp_source)

            block \
                .nl().line('POLYMORPHIC_HELPERS').include("utils/language/polymorphic_helpers.h").nl() \
                .declare_fn('eventName', 'QString', const=True, virtual=True, abstract=True)

            gen_base_debug_write_method(cpp_source, base_event_config)

        struct.nl()

        with struct.protected_block() as _:
            with cpp_source.method(
                class_name,
                'writeDetailsToDebugStream',
                'void',
                ('QDebug UNUSED', 'stream'),
                const=True, virtual=True, declare=True
            ) as _:
                pass

        base_event_config.gen_private_block(struct)

    with cpp_source.function(
        'operator<< ', 'QDebug', ('QDebug', 'stream'), ('CPTR(RawEvent)', 'event'), declare=True
    ) as method:
        method \
            .line('event->writeToDebugStream(stream);') \
            .line('return stream;')

    return h_source


def gen_debug_write_method(cpp_source, class_name, event_config):
    with cpp_source.method(
        class_name,
        'write' + ('FailableEvent' if event_config.fail_reason_enum is not None else '') + 'DetailsToDebugStream',
        'void',
        ('QDebug' + (' UNUSED' if len(event_config.fields) == 0 else ''), 'stream'),
        const=True, virtual=True, declare=True
    ) as method:
        if event_config.custom_debug_write_method:
            method.custom_section('Debug write method')
        else:
            event_config.gen_debug_write_field_code(method, event_config.fields)


def gen_base_debug_write_method(cpp_source, base_event_config):
    time_field = None
    index_field = None

    remaining_fields = []
    for field_config in base_event_config.fields:
        if field_config.name == 'timestamp' and time_field is None:
            time_field = field_config
        elif field_config.name.startswith('index') and index_field is None:
            index_field = field_config
        else:
            remaining_fields.append(field_config)

    with cpp_source.method(
        'RawEvent', 'writeToDebugStream', 'void', ('QDebug', 'stream'), const=True, declare=True
    ) as method:
        method \
            .field('QDebugStateSaver', 'saver(stream)') \
            .line('stream.nospace();').nl() \
            .line('stream << "#" << {0} << " ";'.format(index_field.as_print_rvalue(cpp_source))) \
            .line('stream << "[" << {0} << "] ";'.format(time_field.as_print_rvalue(cpp_source))).nl() \
            .line('stream << QP(eventName());').use_symbol('QP') \
            .line('writeDetailsToDebugStream(stream);').nl()

        base_event_config.gen_debug_write_field_code(method, remaining_fields)


class AbstractEventConfigAugment(GenericPolymorphicAugment):
    def __init__(self, event_config, autogen_core):
        assert isinstance(event_config, RawEventConfig), 'Augmented object should be RawEventConfig'

        GenericPolymorphicAugment.__init__(self, event_config, autogen_core, field_augment=EventFieldAugment)


class BaseEventConfigAugment(AbstractEventConfigAugment):
    def __init__(self, event_config, autogen_core):
        AbstractEventConfigAugment.__init__(self, event_config, autogen_core)

    def mandatory_base_fields(self):
        return list()

    def class_name(self):
        return 'RawEvent'

    def parent_class(self, no_template=None):
        return None


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


class EventFieldAugment(GenericPolymorphicFieldAugment):
    def __init__(self, field_config, autogen_core):
        assert isinstance(field_config, RawEventFieldConfig), 'Augmented object should be RawEventFieldConfig'

        GenericPolymorphicFieldAugment.__init__(self, field_config, autogen_core)
