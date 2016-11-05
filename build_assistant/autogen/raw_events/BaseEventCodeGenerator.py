# build_assistant/autogen/raw_events/BaseEventCodeGenerator.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

from build_assistant.autogen.raw_events.AbstractEventCodeGenerator import AbstractEventCodeGenerator


class BaseEventCodeGenerator(AbstractEventCodeGenerator):
    def __init__(self, event_config, autogen_core):
        AbstractEventCodeGenerator.__init__(self, event_config, autogen_core)

    def mandatory_base_fields(self):
        return list()

    def class_name(self):
        return 'RawEvent'

    def parent_class(self, no_template=None):
        return 'StandardPolymorphic' if no_template else 'StandardPolymorphic<{0}>'.format(self.subtype_enum())

    def gen_code_impl(self, cpp_source, h_source, public_block, protected_block, private_block):
        with public_block as block:
            self.gen_field_declarations(block)
            self.gen_constructors(cpp_source.code, block)
            self.gen_mandatory_fields_sanity_check_method(cpp_source.code, private_block)

            block.nl()
            self.gen_event_name_method(cpp_source.code, block)
            block.nl()
            self.gen_debug_write_method(cpp_source.code, block)

        self.gen_serialize_fields_method(cpp_source.code, protected_block)
        protected_block.nl()
        self.gen_debug_write_details_method(cpp_source.code, protected_block)

    def gen_event_name_method(self, cpp_code, block):
        with cpp_code.method(
            self.class_name(), 'eventName', 'QString', const=True, virtual=True, declare_in=block
        ) as method:
            method.ret('name_for_raw_event_sub_type(subType())')

    def gen_serialize_fields_method(self, cpp_code, protected_block):
        with cpp_code.method(
            self.class_name(), 'serializeToStreamImpl', 'void', ('QDataStream& UNUSED', 'mut_stream'),
            const=True, virtual=True, declare_in=protected_block
        ) as _:
            pass

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
