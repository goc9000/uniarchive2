# build_assistant/AutoGeneratedSource.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import hashlib
import json
import os
import re

from collections import namedtuple
from contextlib import contextmanager

from build_assistant.VirtualPath import VirtualPath
from build_assistant.util import fail


HASH_LINE_PREFIX = ' * Codegen hash (sha256):'


CodeGenConfig = namedtuple('CodeGenConfig', ['app_metadata', 'indent_size', 'gutter_width', 'base_namespace'])


class AutoGeneratedSource:
    virtual_path = None
    name = None
    type = None

    includes = None
    qt_includes = None
    std_includes = None

    uses = None
    std_uses = None

    code_lines = None
    indent_level = 0

    codegen_cfg = None
    type_registry = None

    def __init__(self, virtual_path, name, type, codegen_cfg, type_registry):
        self.virtual_path = virtual_path
        self.name = name
        self.type = type

        self.codegen_cfg = codegen_cfg
        self.type_registry = type_registry

        self.includes = list()
        self.qt_includes = list()
        self.std_includes = list()

        self.uses = list()
        self.std_uses = list()

        self.code_lines = list()

        if type == 'cpp':
            self.includes.append(self.physical_path().replace_basename(name + '.h'))

    def include(self, *items):
        return self._generic_include(self.includes, items)

    def qt_include(self, *items):
        return self._generic_include(self.qt_includes, items)

    def std_include(self, *items):
        return self._generic_include(self.std_includes, items)

    def use(self, *items):
        return self._generic_include(self.uses, items, '::')

    def std_use(self, *items):
        return self._generic_include(self.std_uses, items, '::')

    def _generic_include(self, bank, items, alternate_separator=None):
        for item in items:
            if isinstance(item, str):
                if alternate_separator is not None and alternate_separator in item:
                    item = VirtualPath(item.split(alternate_separator))
                else:
                    item = VirtualPath.from_text(item)
            else:
                assert isinstance(item, VirtualPath)

            if item in bank:
                continue

            bank.append(item)

        return self

    def line(self, line):
        self.code_lines.append((self.get_indent() + line).rstrip())
        return self

    def nl(self):
        return self.line('')

    def line_comment(self, comment):
        for line in comment.split("\n"):
            self.line('// ' + line)
        return self

    def indent(self):
        self.indent_level += 1
        return self

    def unindent(self):
        self.indent_level -= 1
        return self

    def declare_fn(self, function_name, return_type, *param_types):
        # TODO: add mechanics for splitting over several lines if params are too long
        self.line(return_type + ' ' + function_name + '(' + ', '.join(param_types) + ');')
        return self

    @contextmanager
    def block(self, head, semicolon=False, nl_after=True):
        self.line(head + ' {').indent()
        yield self
        self.unindent().line('};' if semicolon else '}')
        if nl_after:
            self.nl()

    def enum_class_block(self, name):
        return self.block('enum class ' + name, semicolon=True)

    def struct_block(self, name, inherits=None):
        inherits = inherits or list()

        head = 'struct ' + name
        if len(inherits) > 0:
            head += ' : ' + ', '.join(inherits)

        return self.block(head, semicolon=True)

    def switch_block(self, switch_by):
        return self.block('switch (' + switch_by + ')')

    @contextmanager
    def case_block(self, case_label):
        self.line('case ' + case_label + ':').indent()
        yield self
        self.unindent()

    @contextmanager
    def colon_block(self, kind):
        self.unindent().line(kind + ':').indent()
        yield self

    def public_block(self):
        return self.colon_block('public')

    def private_block(self):
        return self.colon_block('private')

    def protected_block(self):
        return self.colon_block('protected')

    def function(self, function_name, return_type, *param_types):
        # TODO: add mechanics for splitting over several lines if params are too long
        return self.block(return_type + ' ' + function_name + '(' + ', '.join(param_types) + ')')

    def field(self, type, name):
        self.add_includes_for_type(type)

        return self.line('{0} {1};'.format(type, name))

    def add_includes_for_type(self, type):
        for part in re.split(r'[^A-Za-z0-9_]+', type):
            if part == '':
                continue

            type_info = self.type_registry.lookup(part)

            if type_info.include is not None:
                self.include(type_info.include)
            if type_info.qt_include is not None:
                self.qt_include(type_info.qt_include)
            if type_info.use is not None:
                self.use(type_info.use)

        return self

    def string_literal(self, text):
        return json.dumps(text)  # Eh, probably works ok in most situations

    def get_indent(self):
        return ' ' * (self.indent_level * self.codegen_cfg.indent_size)

    def gen_header_lines(self):
        def gen_lines_real():
            yield self.physical_path().to_text()
            yield ''

            yield 'WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.'
            yield HASH_LINE_PREFIX[3:]
            yield ''

            yield self.codegen_cfg.app_metadata.copyright_text
            yield ''
            yield 'This file is part of {0}.'.format(self.codegen_cfg.app_metadata.app_name)
            yield ''
            yield self.codegen_cfg.app_metadata.license_text

        yield '/**'
        for line in gen_lines_real():
            yield ' * ' + line if line != '' else ' *'
        yield ' */'

    def gen_includes_lines(self):
        for include_group in [self.includes, self.qt_includes, self.std_includes]:
            if len(include_group) == 0:
                continue

            for item in include_group:
                yield ('#include ' + ('"{0}"' if include_group == self.includes else '<{0}>')).format(item)

            yield ''

    def gen_uses_lines(self):
        for use_group in [self.std_uses, self.uses]:
            if len(use_group) == 0:
                continue

            for item in use_group:
                yield 'using namespace {0};'.format('::'.join(item.elements))

            yield ''

    def gen_namespaced_lines(self):
        for line in self.gen_uses_lines():
            yield line

        for line in self.code_lines:
            yield line

    def physical_path(self):
        return VirtualPath(item.strip('()') for item in self.virtual_path.elements).add(self.name + '.' + self.type)

    def namespace_path(self):
        return [self.codegen_cfg.base_namespace] + [x for x in self.virtual_path.elements if not x.startswith('(')]

    def gen_guarded_lines(self):
        for line in self.gen_includes_lines():
            yield line

        yield ''
        namespace_decls = ['namespace {0} {{'.format(namespace) for namespace in self.namespace_path()]
        yield ' '.join(namespace_decls)
        yield ''

        for line in self.gen_namespaced_lines():
            yield line

        yield ''
        yield '}' * len(namespace_decls)

    def gen_guards(self, guarded_lines):
        guard_name = '_'.join(self.namespace_path() + [self.name, 'h']).upper()

        yield '#ifndef ' + guard_name
        yield '#define ' + guard_name
        yield ''

        for line in guarded_lines:
            yield line

        yield ''
        yield '#endif //' + guard_name

    def gen_body_lines(self):
        guarded_lines = self.gen_guarded_lines()

        for line in self.gen_guards(guarded_lines) if self.type == 'h' else guarded_lines:
            yield line

    def gen_unfiltered_lines(self):
        for line in self.gen_header_lines():
            yield line

        yield ''

        for line in self.gen_body_lines():
            yield line

    def gen_lines_without_hash(self):
        for line in filter_empty_lines(self.gen_unfiltered_lines()):
            yield line

    def gen_lines(self):
        content_hash = compute_hash(self.gen_lines_without_hash())

        for line in self.gen_lines_without_hash():
            if line.startswith(HASH_LINE_PREFIX):
                yield line + ' ' + content_hash
            else:
                yield line

    def commit(self, base_src_path):
        real_path = base_src_path.append(self.physical_path()).realize()

        os.makedirs(base_src_path.append(self.physical_path().parent()).realize(), exist_ok=True)

        if os.path.isfile(real_path):
            check_existing_generated_file(real_path)

        with open(real_path, 'wt') as f:
            f.writelines(line + "\n" for line in self.gen_lines())


def check_existing_generated_file(real_path):
    declared_hash = None
    hasher = hashlib.sha256()

    with open(real_path, 'rt') as f:
        for line in f.readlines():
            if line.endswith("\n"):
                line = line[:-1]

            if line.startswith(HASH_LINE_PREFIX):
                declared_hash = line[len(HASH_LINE_PREFIX)+1:]
            else:
                hasher.update(line.encode('ascii'))

    if declared_hash is None:
        fail(
            """
            No line specifying the content hash found for {0}. It might not be an autogenerated file!
            Please double-check the autogeneration config. Aborting so as not to accidentally overwrite manual files.
            """,
            real_path
        )

    actual_hash = hasher.hexdigest()

    if actual_hash != declared_hash:
        fail(
            """
            Content hash is incorrect for {0}. The file has been modified manually.
            Please save any manual changes so they can be applied elsewhere, and delete the file so it can be regenerated.
            """,
            real_path
        )


def compute_hash(lines):
    hasher = hashlib.sha256()
    hash_line_found = False

    for line in lines:
        if line.startswith(HASH_LINE_PREFIX):
            hash_line_found = True
        else:
            hasher.update(line.encode('ascii'))

    assert hash_line_found

    return hasher.hexdigest()


def filter_empty_lines(lines):
    """Collapses multiple empty lines into one, and filters out trailing empty lines"""
    has_empty_line = False
    for line in lines:
        if line != '':
            if has_empty_line:
                yield ''
                has_empty_line = False
            yield line
        else:
            has_empty_line = True
