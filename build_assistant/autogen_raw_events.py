# build_assistant/autogen_raw_events.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from collections import namedtuple

from build_assistant.VirtualPath import VirtualPath
from build_assistant.SymbolRegistry import TypeKind
from build_assistant.grammar import camelcase_to_underscore, singular


ConstructorInfo = namedtuple('ConstructorInfo', ['params', 'subconstructors', 'init_statements'])


BASE_EVENTS_PATH = VirtualPath(['intermediate_format', 'events'])


def autogen_raw_events_index(autogen_config):
    yield BASE_EVENTS_PATH, 'RawEvent'

    for path, name, _ in autogen_config.raw_events:
        yield BASE_EVENTS_PATH.append(path), 'Raw' + name + 'Event'

    return []


def gen_raw_events(autogen_config, autogen_core):
    base_event_config = BaseEventConfigWrapper(autogen_config.base_raw_event, autogen_core)

    base_event_h = gen_base_raw_event(base_event_config, autogen_core)

    for rel_path, name, event_config in autogen_config.raw_events:
        event_config = EventConfigWrapper(name, event_config, autogen_core, base_config=base_event_config)
        class_name = event_config.class_name()

        cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH.append(rel_path), class_name)

        with h_source.struct_block(class_name, inherits=[event_config.parent_class()]) as struct:
            h_source.cover_symbols_from(base_event_h)

            with struct.public_block() as block:
                if len(event_config.fields) > 0:
                    for index, field in enumerate(event_config.fields):
                        if index in event_config.field_breaks:
                            block.nl()

                        if field.doc is not None:
                            block.doc_comment(field.doc)

                        block.field(*field.as_field_decl())

                    block.nl()

                for ctor_info in event_config.constructors():
                    with cpp_source.constructor(
                        class_name, *ctor_info.params, inherits=ctor_info.subconstructors, declare=True
                    ) as cons:
                        for line in ctor_info.init_statements:
                            cons.line(line)

                if event_config.has_mandatory_fields_sanity_check():
                    gen_sanity_check_for_mandatory_params(cpp_source, class_name, event_config)

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

            if event_config.has_mandatory_fields_sanity_check():
                struct.nl()
                with struct.private_block() as block:
                    block.declare_fn('sanityCheckMandatoryParameters', 'void', const=True)


def gen_base_raw_event(base_event_config, autogen_core):
    class_name = base_event_config.class_name()

    cpp_source, h_source = autogen_core.new_pair(BASE_EVENTS_PATH, class_name)

    with h_source.struct_block(class_name) as struct:
        with struct.public_block() as block:
            if len(base_event_config.fields) > 0:
                for index, field in enumerate(base_event_config.fields):
                    if index in base_event_config.field_breaks:
                        block.nl()

                    if field.doc is not None:
                        block.doc_comment(field.doc)

                    block.field(*field.as_field_decl())

                block.nl()

            for ctor_info in base_event_config.constructors():
                with cpp_source.constructor(
                    class_name, *ctor_info.params, inherits=ctor_info.subconstructors, declare=True
                ) as cons:
                    for line in ctor_info.init_statements:
                        cons.line(line)

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


def gen_sanity_check_for_mandatory_params(cpp_source, class_name, event_config):
    with cpp_source.method(class_name, 'sanityCheckMandatoryParameters', 'void', const=True) as method:
        for field in event_config.fields:
            if not (field.is_mandatory() and field.is_checkable()):
                continue

            field.write_param_check(method)


class AbstractEventConfigWrapper:
    _event_config = None
    _core = None

    def __init__(self, event_config, autogen_core):
        self._event_config = event_config._replace(
            fields=[EventFieldWrapper(field, autogen_core) for field in event_config.fields]
        )
        self._core = autogen_core

    def __getattr__(self, name):
        return getattr(self._event_config, name)

    def _replace(self, **_):
        assert False, 'Wrappers do not support _replace'

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
            sanity_check_statements = ['sanityCheckMandatoryParameters();'] \
                if self.has_mandatory_fields_sanity_check() else list()

            yield ConstructorInfo(
                params=[f.as_param() for f in base_fields + inited_fields],
                subconstructors=parent_constructor + [f.as_subconstructor() for f in inited_fields],
                init_statements=sanity_check_statements,
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
                        ] + sanity_check_statements
                    )
                    break

            field_config = next(maybe_addable_fields, None)
            if field_config is None:
                break

            extra_enabled_fields.add(field_config.name)

    def has_mandatory_fields_sanity_check(self):
        return any(f.is_mandatory() and f.is_checkable() for f in self.fields)


class BaseEventConfigWrapper(AbstractEventConfigWrapper):
    def __init__(self, event_config, autogen_core):
        AbstractEventConfigWrapper.__init__(self, event_config, autogen_core)

    def mandatory_base_fields(self):
        return list()

    def class_name(self):
        return 'RawEvent'

    def parent_class(self, no_template=None):
        return None


class EventConfigWrapper(AbstractEventConfigWrapper):
    _name = None
    _base_config = None

    def __init__(self, name, event_config, autogen_core, base_config=None):
        AbstractEventConfigWrapper.__init__(self, event_config, autogen_core)
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


class EventFieldWrapper:
    _field_config = None
    _core = None

    _base_type = None
    _type_info = None

    def __init__(self, field_config, autogen_core):
        self._field_config = field_config
        self._core = autogen_core

        self._base_type = field_config.base_type
        self._type_info = autogen_core.symbol_registry.lookup(self._base_type)
        assert self._type_info.is_type, '{0} is not a base type'.format(self._base_type)

    def __getattr__(self, name):
        return getattr(self._field_config, name)

    def _replace(self, **_):
        assert False, 'Wrappers do not support _replace'

    def local_name(self):
        return self.short_name or camelcase_to_underscore(self.name)

    def as_field_decl(self):
        type_kind = self._type_info.type_kind

        use_optional = self.is_optional and not (type_kind == TypeKind.POLYMORPHIC and not self.is_list)
        use_unique_ptr = type_kind == TypeKind.POLYMORPHIC
        use_vector = self.is_list

        cpp_type = self._base_type
        if use_unique_ptr:
            cpp_type = 'unique_ptr<{0}>'.format(cpp_type)
        if use_vector:
            cpp_type = 'vector<{0}>'.format(cpp_type)
        if use_optional:
            cpp_type = 'optional<{0}>'.format(cpp_type)

        return cpp_type, self.name, self.default_value

    def as_param(self):
        type_kind = self._type_info.type_kind

        cpp_type = self._base_type
        if type_kind == TypeKind.POLYMORPHIC:
            cpp_type = ('TAKE_VEC({0})' if self.is_list else 'TAKE({0})').format(cpp_type)
        else:
            if self.is_list:
                cpp_type = 'vector<{0}>'.format(cpp_type)

            if type_kind == TypeKind.MOVABLE:
                cpp_type += '&&'
            elif type_kind != TypeKind.PRIMITIVE or self.is_list:
                cpp_type = 'IMM({0})'.format(cpp_type)

        return cpp_type, self.local_name()

    def as_rvalue(self):
        type_kind = self._type_info.type_kind

        rvalue = self.local_name()

        if type_kind == TypeKind.POLYMORPHIC or type_kind == TypeKind.MOVABLE:
            rvalue = 'move({0})'.format(rvalue)

        return rvalue

    def as_subconstructor(self):
        return '{0}({1})'.format(self.name, self.as_rvalue())

    def as_print_rvalue(self, source):
        rvalue_expr = self.name

        if self._type_info.type_kind == TypeKind.POLYMORPHIC and not self.is_list:
            rvalue_expr += '.get()'
        else:
            if self.is_optional:
                rvalue_expr = '*' + rvalue_expr
            if self.is_list:
                source.include("utils/qt/debug_extras.h")  # For printing vectors

        return rvalue_expr

    def singularized(self):
        return EventFieldWrapper(
            self._field_config._replace(
                is_list=False,
                name=singular(self.name),
                short_name=singular(self.short_name) if self.short_name is not None else None
            ),
            self._core
        )

    def is_mandatory(self):
        return not self.is_optional and self.default_value is None

    def is_checkable(self):
        type_kind = self._type_info.type_kind

        return type_kind == TypeKind.POLYMORPHIC

    def write_param_check(self, source):
        type_kind = self._type_info.type_kind

        if type_kind == TypeKind.POLYMORPHIC:
            if self.is_list:
                with source.for_each_block('IMM(auto)', 'item', self.name, nl_after=False) as block:
                    block.call(
                        'invariant',
                        'item',
                        source.string_literal("Parameter '{0}' cannot have empty entries".format(self.local_name()))
                    )
            else:
                source.call(
                    'invariant',
                    self.name,
                    source.string_literal("Parameter '{0}' cannot have empty value".format(self.local_name()))
                )
