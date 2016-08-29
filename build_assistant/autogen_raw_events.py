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
from build_assistant.autogen_common import get_full_autogen_raw_event_path_and_name
from build_assistant.grammar import camelcase_to_underscore, singular


ConstructorInfo = namedtuple('ConstructorInfo', ['params', 'subconstructors', 'init_statements'])


def gen_raw_events(autogen_config, autogen_core):
    def local_name(field_config):
        return field_config.short_name or camelcase_to_underscore(field_config.name)

    def as_field_decl(field_config):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        use_optional = field_config.is_optional and not \
            (type_info.type_kind == TypeKind.POLYMORPHIC and not field_config.is_list)
        use_unique_ptr = type_info.type_kind == TypeKind.POLYMORPHIC
        use_vector = field_config.is_list

        cpp_type = base_type
        if use_unique_ptr:
            cpp_type = 'unique_ptr<{0}>'.format(cpp_type)
        if use_vector:
            cpp_type = 'vector<{0}>'.format(cpp_type)
        if use_optional:
            cpp_type = 'optional<{0}>'.format(cpp_type)

        return cpp_type, field_config.name, field_config.default_value

    def as_param(field_config):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        cpp_type = base_type

        if type_info.type_kind == TypeKind.POLYMORPHIC:
            cpp_type = ('TAKE_VEC({0})' if field_config.is_list else 'TAKE({0})').format(cpp_type)
        else:
            if field_config.is_list:
                cpp_type = 'vector<{0}>'.format(cpp_type)

            if type_info.type_kind == TypeKind.MOVABLE:
                cpp_type += '&&'
            elif type_info.type_kind != TypeKind.PRIMITIVE or field_config.is_list:
                cpp_type = 'IMM({0})'.format(cpp_type)

        return cpp_type, local_name(field_config)

    def as_rvalue(field_config):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        rvalue = local_name(field_config)

        if type_info.type_kind == TypeKind.POLYMORPHIC or type_info.type_kind == TypeKind.MOVABLE:
            rvalue = 'move({0})'.format(rvalue)

        return rvalue

    def as_subconstructor(field_config):
        return '{0}({1})'.format(field_config.name, as_rvalue(field_config))

    def as_print_rvalue(field_config, source):
        base_type = field_config.base_type
        type_info = autogen_core.symbol_registry.lookup(base_type)
        assert type_info.is_type, '{0} is not a base type'.format(base_type)

        rvalue_expr = field_config.name

        if type_info.type_kind == TypeKind.POLYMORPHIC and not field_config.is_list:
            rvalue_expr += '.get()'
        else:
            if field_config.is_optional:
                rvalue_expr = '*' + rvalue_expr
            if field_config.is_list:
                source.include("utils/qt/debug_extras.h")  # For printing vectors

        return rvalue_expr

    def is_mandatory_field(field_config):
        return not field_config.is_optional and field_config.default_value is None

    def constructors(event_config):
        if event_config is not None:
            base_fields = list(filter(is_mandatory_field, autogen_config.base_raw_event.fields))
            free_fields = event_config.fields

            parent_class = 'RawEvent' if not is_failable else 'RawFailableEvent'
            parent_constructor = [parent_class + '(' + ', '.join(map(as_rvalue, base_fields)) + ')']
        else:
            base_fields = []
            free_fields = autogen_config.base_raw_event.fields
            parent_constructor = []

        maybe_addable_fields = filter(lambda f: f.add_to_constructor, free_fields)
        extra_enabled_fields = set()

        while True:
            inited_fields = [f for f in free_fields if f.name in extra_enabled_fields or is_mandatory_field(f)]

            yield ConstructorInfo(
                params=[as_param(f) for f in base_fields + inited_fields],
                subconstructors=parent_constructor + [as_subconstructor(f) for f in inited_fields],
                init_statements=list(),
            )

            # Generate convenience constructor for the first singularizable field
            for index, field_config in enumerate(inited_fields):
                if field_config.maybe_singleton:
                    singularized = field_config._replace(
                        is_list=False,
                        name=singular(field_config.name),
                        short_name=singular(field_config.short_name) if field_config.short_name is not None else None
                    )

                    params = \
                        [as_param(f) for f in base_fields + inited_fields[:index]] + \
                        [as_param(singularized)] + \
                        [as_param(f) for f in inited_fields[index+1:]]

                    subcons = parent_constructor + \
                        [as_subconstructor(f) for f in inited_fields[:index]] + \
                        [as_subconstructor(f) for f in inited_fields[index+1:]]

                    yield ConstructorInfo(
                        params=params,
                        subconstructors=subcons,
                        init_statements=[
                            '{0}.push_back({1});'.format(field_config.name, as_rvalue(singularized))
                        ]
                    )
                    break

            field_config = next(maybe_addable_fields, None)
            if field_config is None:
                break

            extra_enabled_fields.add(field_config.name)

    def gen_debug_write_field_code(method, fields):
        def commit_regular_fields(block, regular_fields_line):
            if regular_fields_line is not None:
                block.line(regular_fields_line + ';')

        def write_regular_field(block, regular_fields_line, field_config):
            if regular_fields_line is None:
                regular_fields_line = 'stream'

            added_text = ' << " {0}=" << {1}'.format(local_name(field_config), as_print_rvalue(field_config, block))

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
                name = local_name(field_config)
                rvalue = as_print_rvalue(field_config, block)

                with block.if_block('{0}.size() == 1'.format(field_config.name), nl_after=False) as b:
                    b.line('stream << " {0}=" << {1}.front();'.format(singular(name), rvalue))
                    with b.else_block() as e:
                        b.line('stream << " {0}=" << {1};'.format(name, rvalue))
            else:
                write_irregular_field3(block, field_config)

        def write_irregular_field3(block, field_config):
            block.line(
                'stream << " {0}=" << {1};'.format(local_name(field_config), as_print_rvalue(field_config, block))
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

    def gen_debug_write_method(cpp_source, event_config):
        with cpp_source.method(
            class_name,
            debug_write_method,
            'void',
            ('QDebug' + (' UNUSED' if len(event_config.fields) == 0 else ''), 'stream'),
            const=True
        ) as method:
            if event_config.custom_debug_write_method:
                method.custom_section('Debug write method')
            else:
                gen_debug_write_field_code(method, event_config.fields)

    def gen_base_raw_event():
        base_path = VirtualPath(['intermediate_format', 'events'])
        class_name = 'RawEvent'

        cpp_source, h_source = autogen_core.new_pair(base_path, class_name)

        def gen_base_debug_write_method():
            time_field = None
            index_field = None

            remaining_fields = []
            for field_config in autogen_config.base_raw_event.fields:
                if field_config.name == 'timestamp' and time_field is None:
                    time_field = field_config
                elif field_config.name.startswith('index') and index_field is None:
                    index_field = field_config
                else:
                    remaining_fields.append(field_config)

            with cpp_source.method(
                class_name, 'writeToDebugStream', 'void', ('QDebug', 'stream'), const=True
            ) as method:
                method \
                    .field('QDebugStateSaver', 'saver(stream)') \
                    .line('stream.nospace();').nl() \
                    .line('stream << "#" << {0} << " ";'.format(as_print_rvalue(index_field, cpp_source))) \
                    .line('stream << "[" << {0} << "] ";'.format(as_print_rvalue(time_field, cpp_source))).nl() \
                    .line('stream << QP(eventName());').add_includes_for_type('QP') \
                    .line('writeDetailsToDebugStream(stream);').nl()

                gen_debug_write_field_code(method, remaining_fields)

        with h_source.struct_block(class_name) as struct:
            with struct.public_block() as block:
                if len(autogen_config.base_raw_event.fields) > 0:
                    for index, field in enumerate(autogen_config.base_raw_event.fields):
                        if index in autogen_config.base_raw_event.field_breaks:
                            block.nl()

                        if field.doc is not None:
                            block.doc_comment(field.doc)

                        block.field(*as_field_decl(field))

                    block.nl()

                for ctor_info in constructors(None):
                    block.declare_constructor(class_name, *ctor_info.params)

                block \
                    .nl().line('POLYMORPHIC_HELPERS').include("utils/language/polymorphic_helpers.h").nl() \
                    .declare_fn('eventName', 'QString', const=True, virtual=True, abstract=True) \
                    .declare_fn('writeToDebugStream', 'void', ('QDebug', 'stream'), const=True)

            struct.nl()

            with struct.protected_block() as block:
                block.declare_fn(
                    'writeDetailsToDebugStream',
                    'void', ('QDebug', 'stream'),
                    virtual=True,
                    const=True
                )

        h_source.declare_fn('operator<< ', 'QDebug', ('QDebug', 'stream'), ('CPTR(RawEvent)', 'event'))

        cpp_source.cover_symbols(h_source.get_covered_symbols())

        for ctor_info in constructors(None):
            with cpp_source.constructor(class_name, *ctor_info.params, inherits=ctor_info.subconstructors) as cons:
                for line in ctor_info.init_statements:
                    cons.line(line)

        gen_base_debug_write_method()

        with cpp_source.method(
            class_name, 'writeDetailsToDebugStream', 'void', ('QDebug UNUSED', 'stream'), const=True
        ) as _:
            pass

        with cpp_source.function('operator<< ', 'QDebug', ('QDebug', 'stream'), ('CPTR(RawEvent)', 'event')) as method:
            method \
                .line('event->writeToDebugStream(stream);') \
                .line('return stream;')

        return h_source

    base_event_h = gen_base_raw_event()

    for path, name, event_config in autogen_config.raw_events:
        path, class_name = get_full_autogen_raw_event_path_and_name(path, name)

        is_failable = event_config.fail_reason_enum is not None

        cpp_source, h_source = autogen_core.new_pair(path, class_name)

        parent_class = 'RawEvent' if not is_failable else 'RawFailableEvent<' + event_config.fail_reason_enum + '>'
        debug_write_method = 'write' + ('FailableEvent' if is_failable else '') + 'DetailsToDebugStream'

        with h_source.struct_block(class_name, inherits=[parent_class]) as struct:
            h_source.cover_symbols(base_event_h.get_covered_symbols())

            with struct.public_block() as block:
                if len(event_config.fields) > 0:
                    for index, field in enumerate(event_config.fields):
                        if index in event_config.field_breaks:
                            block.nl()

                        if field.doc is not None:
                            block.doc_comment(field.doc)

                        block.field(*as_field_decl(field))

                    block.nl()

                for ctor_info in constructors(event_config):
                    block.declare_constructor(class_name, *ctor_info.params)

                block.nl().declare_fn('eventName', 'QString', const=True, virtual=True)

            struct.nl()

            with struct.protected_block() as block:
                block.declare_fn(debug_write_method, 'void', ('QDebug', 'stream'), virtual=True, const=True)

        cpp_source.cover_symbols(h_source.get_covered_symbols())

        for ctor_info in constructors(event_config):
            with cpp_source.constructor(class_name, *ctor_info.params, inherits=ctor_info.subconstructors) as cons:
                for line in ctor_info.init_statements:
                    cons.line(line)

        with cpp_source.method(class_name, 'eventName', 'QString', const=True) as method:
            if event_config.custom_name_method:
                method.custom_section('Name method')
            else:
                method.line("return {0};".format(method.string_literal(name)))

        gen_debug_write_method(cpp_source, event_config)
