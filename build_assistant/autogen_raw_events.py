# build_assistant/autogen_raw_events.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from collections import namedtuple

from build_assistant.Augment import Augment
from build_assistant.VirtualPath import VirtualPath
from build_assistant.grammar import singular
from build_assistant.AutoGenConfig import RawEventConfig, RawEventFieldConfig
from build_assistant.GenericPolymorphicFieldAugment import GenericPolymorphicFieldAugment


ConstructorInfo = namedtuple('ConstructorInfo', ['params', 'subconstructors', 'init_statements'])


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
            gen_debug_write_field_code(method, event_config.fields)


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

        gen_debug_write_field_code(method, remaining_fields)


def gen_debug_write_field_code(method, fields):
    def commit_regular_fields(block, regular_fields_line):
        if regular_fields_line is not None:
            block.line(regular_fields_line + ';')

    def write_regular_field(block, regular_fields_line, field_config):
        if regular_fields_line is None:
            regular_fields_line = 'stream'

        added_text = ' << " {0}=" << {1}'.format(field_config.local_name(), field_config.as_print_rvalue(block))

        if not block.line_fits(regular_fields_line + added_text + ';'):
            commit_regular_fields(block, regular_fields_line)
            regular_fields_line = 'stream'

        regular_fields_line += added_text

        return regular_fields_line

    def write_irregular_field(block, field_config):
        if field_config.is_optional:
            with block.if_block(field_config.name, nl_after=False) as b:
                write_irregular_field2(b, field_config)
        else:
            write_irregular_field2(block, field_config)

    def write_irregular_field2(block, field_config):
        if field_config.maybe_singleton:
            name = field_config.local_name()
            rvalue = field_config.as_print_rvalue(block)

            with block.if_block('{0}.size() == 1'.format(field_config.name), nl_after=False) as b:
                b.line('stream << " {0}=" << {1}.front();'.format(singular(name), rvalue))
                with b.else_block() as e:
                    b.line('stream << " {0}=" << {1};'.format(name, rvalue))
        else:
            write_irregular_field3(block, field_config)

    def write_irregular_field3(block, field_config):
        block.line(
            'stream << " {0}=" << {1};'.format(field_config.local_name(), field_config.as_print_rvalue(block))
        )

    regular_fields_line = None

    for field_config in fields:
        # First, write regular fields
        if not (field_config.is_optional or field_config.maybe_singleton):
            regular_fields_line = write_regular_field(method, regular_fields_line, field_config)
            continue

        commit_regular_fields(method, regular_fields_line)
        regular_fields_line = None

        write_irregular_field(method, field_config)

    commit_regular_fields(method, regular_fields_line)


class AbstractEventConfigAugment(Augment):
    _core = None

    def __init__(self, event_config, autogen_core):
        Augment.__init__(
            self,
            event_config._replace(
                fields=[EventFieldAugment(field, autogen_core) for field in event_config.fields]
            ),
            RawEventConfig
        )

        self._core = autogen_core

    def mandatory_base_fields(self):
        raise NotImplementedError

    def class_name(self):
        raise NotImplementedError

    def parent_class(self, no_template=None):
        raise NotImplementedError

    def constructors(self):
        base_fields = self.mandatory_base_fields()
        free_fields = self.fields
        parent_constructor = \
            [self.parent_class(no_template=True) + '(' + ', '.join(f.as_rvalue() for f in base_fields) + ')'] \
            if self.parent_class() is not None else []

        maybe_addable_fields = filter(lambda f: f.add_to_constructor, free_fields)
        extra_enabled_fields = set()

        while True:
            inited_fields = [f for f in free_fields if f.name in extra_enabled_fields or f.is_mandatory()]

            yield ConstructorInfo(
                params=[f.as_param() for f in base_fields + inited_fields],
                subconstructors=parent_constructor + [f.as_subconstructor() for f in inited_fields],
                init_statements=list(),
            )

            # Generate convenience constructor for the first singularizable field
            for index, field_config in enumerate(inited_fields):
                if field_config.maybe_singleton:
                    singularized = field_config.singularized()

                    params = \
                        [f.as_param() for f in base_fields + inited_fields[:index]] + \
                        [singularized.as_param()] + \
                        [f.as_param() for f in inited_fields[index + 1:]]

                    subcons = parent_constructor + \
                              [f.as_subconstructor() for f in inited_fields[:index]] + \
                              [f.as_subconstructor() for f in inited_fields[index + 1:]]

                    yield ConstructorInfo(
                        params=params,
                        subconstructors=subcons,
                        init_statements=[
                            '{0}.push_back({1});'.format(field_config.name, singularized.as_rvalue())
                        ],
                    )
                    break

            field_config = next(maybe_addable_fields, None)
            if field_config is None:
                break

            extra_enabled_fields.add(field_config.name)

    def gen_field_declarations(self, block):
        if len(self.fields) > 0:
            for index, field in enumerate(self.fields):
                if index in self.field_breaks:
                    block.nl()

                if field.doc is not None:
                    block.doc_comment(field.doc)

                block.field(*field.as_field_decl())

            block.nl()

    def gen_constructors(self, cpp_source):
        for ctor_info in self.constructors():
            with cpp_source.constructor(
                 self.class_name(), *ctor_info.params, inherits=ctor_info.subconstructors, declare=True
            ) as cons:
                for line in ctor_info.init_statements:
                    cons.line(line)

                if self.has_mandatory_fields_sanity_check():
                    cons.line('sanityCheckMandatoryParameters();')

    def gen_mandatory_fields_sanity_check_method(self, cpp_source):
        if self.has_mandatory_fields_sanity_check():
            with cpp_source.method(self.class_name(), 'sanityCheckMandatoryParameters', 'void', const=True) as method:
                for field in self.fields:
                    if field.is_mandatory() and field.is_checkable():
                        field.gen_param_check(method)

    def gen_private_block(self, struct):
        if self.has_mandatory_fields_sanity_check():
            struct.nl()
            with struct.private_block() as block:
                block.declare_fn('sanityCheckMandatoryParameters', 'void', const=True)

    def has_mandatory_fields_sanity_check(self):
        return any(f.is_mandatory() and f.is_checkable() for f in self.fields)


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
        GenericPolymorphicFieldAugment.__init__(self, field_config, autogen_core)

        assert isinstance(field_config, RawEventFieldConfig), 'Augmented object should be RawEventFieldConfig'
