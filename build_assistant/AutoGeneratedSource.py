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

from build_assistant.includes import IncludeType, UseType, normalize_include_path, normalize_use_path
from build_assistant.VirtualPath import VirtualPath
from build_assistant.util import fail


HASH_LINE_PREFIX = ' * Codegen hash (sha256):'


CodeGenConfig = namedtuple('CodeGenConfig', ['app_metadata', 'indent_size', 'gutter_width', 'base_namespace'])


class AutoGeneratedSource:
    virtual_path = None
    name = None
    type = None

    include_banks = None
    use_banks = None

    covered_symbols = None
    code_lines = None
    indent_level = 0

    codegen_cfg = None
    symbol_registry = None

    def __init__(self, virtual_path, name, type, codegen_cfg, symbol_registry):
        self.virtual_path = virtual_path
        self.name = name
        self.type = type

        self.codegen_cfg = codegen_cfg
        self.symbol_registry = symbol_registry

        self.include_banks = {
            IncludeType.LOCAL: list(),
            IncludeType.STD: list(),
            IncludeType.QT: list(),
        }
        self.use_banks = {
            UseType.LOCAL: list(),
            UseType.STD: list(),
        }

        self.code_lines = list()

        self.covered_symbols = set()

        if type == 'cpp':
            self.include_banks[IncludeType.LOCAL].append(self.physical_path().replace_basename(name + '.h'))

    # Basics

    def line(self, line):
        self.code_lines.append((self._get_indent() + line).rstrip())
        return self

    def nl(self):
        return self.line('')

    # Comments

    def line_comment(self, comment):
        for line in comment.split("\n"):
            self.line('// ' + line)
        return self

    # Indent

    def indent(self):
        self.indent_level += 1
        return self

    def unindent(self):
        self.indent_level -= 1
        return self

    def line_fits(self, line):
        return self.indent_level * self.codegen_cfg.indent_size + len(line) <= self.codegen_cfg.gutter_width

    def _get_indent(self):
        return ' ' * (self.indent_level * self.codegen_cfg.indent_size)

    # Toplevel blocks

    def enum_class_block(self, name):
        return self._generalized_block('enum class ' + name, semicolon=True)

    def struct_block(self, name, inherits=None):
        inherits = inherits or list()

        for item in inherits:
            self.add_includes_for_type(item)

        return self._generalized_block('struct ' + name, inherits=inherits, semicolon=True)

    def public_block(self):
        return self._colon_block('public')

    def private_block(self):
        return self._colon_block('private')

    def protected_block(self):
        return self._colon_block('protected')

    @contextmanager
    def _generalized_block(
        self,
        head,
        params=None,
        param_separator=',',
        inherits=None,
        decorations=None,
        semicolon=False
    ):
        """
        A generalized block looks like this:
        HEAD(param1, param2, param3)[decorations] : inherit1, inherit2, inherit3 {
        }

        or, alternatively, when items don't fit:

        HEAD(
          param1,
          param2
          param3
        )[decorations] : inherit1, inherit2,
                         inherit3 {
        }
        """

        self._generalized_head(
            head,
            params=params,
            param_separator=param_separator,
            inherits=inherits,
            decorations=decorations,
            closer=' {'
        ).indent()

        yield self

        return self.unindent().line('}' + (';' if semicolon else '')).nl()

    def _generalized_head(
        self,
        head,
        params=None,
        param_separator=',',
        inherits=None,
        decorations=None,
        closer=';'
    ):
        if inherits is not None and len(inherits) == 0:
            inherits = None

        # First, try one-liner
        one_line = head
        if params is not None:
            one_line += '(' + (param_separator + ' ').join(params) + ')'
        one_line += decorations or ''
        without_inherits = one_line
        if inherits is not None:
            one_line += ' : ' + ', '.join(inherits)
        one_line += closer

        if self.line_fits(one_line):
            return self.line(one_line)

        if inherits is not None and self.line_fits(without_inherits):  # Then, try to break at the inherits
            self.line(without_inherits)
            inherits_base = ' '
        elif params is not None:  # Try to break at the params
            self.line(head + '(').indent()

            for index, param in enumerate(params):
                self.line(param + (param_separator if index < len(params) - 1 else ''))

            self.unindent()

            inherits_base = ')' + (decorations or '')

            if inherits is None:
                return self.line(inherits_base + closer)
        else:
            assert False, "Head too long: {0}".format(head)

        line = inherits_base + ' : '
        first_in_line = True

        for index, item in enumerate(inherits):
            item_and_sep = item + (',' if index < len(inherits) - 1 else closer)
            try_line = line + ('' if first_in_line else ' ') + item_and_sep
            first_in_line = False

            if not self.line_fits(try_line):
                self.line(line)
                line = ' ' * (len(inherits_base) + 3) + item_and_sep
                first_in_line = True
            else:
                line = try_line

        self.line(line)

        return self

    @contextmanager
    def _colon_block(self, kind):
        self.unindent().line(kind + ':').indent()
        yield self

    # Statement blocks

    def switch_block(self, switch_by):
        return self._generalized_block('switch ', params=[switch_by])

    @contextmanager
    def case_block(self, case_label):
        self.line('case ' + case_label + ':').indent()
        yield self
        self.unindent()

    # Functions

    def declare_fn(self, function_name, return_type, *params, const=False, virtual=False, abstract=False):
        self.add_includes_for_type(return_type)
        for type, _ in params:
            self.add_includes_for_type(type)

        return self._generalized_head(
            ('virtual ' if virtual else '') + return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params],
            decorations=(' const' if const else '') + (' = 0' if abstract else '')
        )

    def declare_constructor(self, class_name, *params):
        for type, _ in params:
            self.add_includes_for_type(type)

        return self._generalized_head(class_name, params=[type + ' ' + name for type, name in params])

    def function(self, function_name, return_type, *params):
        self.add_includes_for_type(return_type)
        for type, _ in params:
            self.add_includes_for_type(type)

        return self._generalized_block(
            return_type + ' ' + function_name,
            params=[type + ' ' + name for type, name in params]
        )

    def constructor(self, class_name, *params, inherits=None):
        for type, _ in params:
            self.add_includes_for_type(type)

        return self._generalized_block(
            class_name + '::' + class_name,
            params=[type + ' ' + name for type, name in params],
            inherits=inherits
        )

    # Fields

    def field(self, type, name):
        self.add_includes_for_type(type)

        return self.line('{0} {1};'.format(type, name))

    # Miscellaneous

    def string_literal(self, text):
        return json.dumps(text)  # Eh, probably works ok in most situations

    # Includes and covered types

    def include(self, *items):
        return self.include_programmatic(IncludeType.LOCAL, *items)

    def std_include(self, *items):
        return self.include_programmatic(IncludeType.STD, *items)

    def qt_include(self, *items):
        return self.include_programmatic(IncludeType.QT, *items)

    def use(self, *items):
        return self.use_programmatic(UseType.LOCAL, *items)

    def std_use(self, *items):
        return self.use_programmatic(UseType.STD, *items)

    def include_programmatic(self, bank, *items):
        for path in map(normalize_include_path, items):
            if bank == IncludeType.LOCAL and path == self.physical_path():
                continue
            if path in self.include_banks[bank]:
                continue

            self.include_banks[bank].append(path)

        return self

    def use_programmatic(self, bank, *items):
        for path in map(normalize_use_path, items):
            if bank == UseType.LOCAL and path.is_equal_or_parent_of(VirtualPath(self.namespace_path())):
                continue
            if path in self.use_banks[bank]:
                continue

            self.use_banks[bank].append(path)

        return self

    def get_covered_symbols(self):
        return set(self.covered_symbols)

    def cover_symbols(self, symbols):
        self.covered_symbols |= set(symbols)
        return self

    def add_includes_for_type(self, type):
        for part in re.split(r'[^A-Za-z0-9_]+', type):
            if part == '':
                continue
            if part in self.covered_symbols:
                continue

            self.covered_symbols.add(part)

            symbol_info = self.symbol_registry.lookup(part)

            if symbol_info.include is not None:
                self.include_programmatic(*symbol_info.include)
            if symbol_info.use is not None:
                self.use_programmatic(*symbol_info.use)

        return self

    # Misc info

    def physical_path(self):
        return VirtualPath(item.strip('()') for item in self.virtual_path.elements).add(self.name + '.' + self.type)

    def namespace_path(self):
        return [self.codegen_cfg.base_namespace] + [x for x in self.virtual_path.elements if not x.startswith('(')]

    # Generation

    def commit(self, base_src_path):
        real_path = base_src_path.append(self.physical_path()).realize()

        os.makedirs(base_src_path.append(self.physical_path().parent()).realize(), exist_ok=True)

        if os.path.isfile(real_path):
            check_existing_generated_file(real_path)

        with open(real_path, 'wt') as f:
            f.writelines(line + "\n" for line in self._gen_lines())

    def _gen_header_lines(self):
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

    def _gen_includes_lines(self):
        for bank in [IncludeType.LOCAL, IncludeType.QT, IncludeType.STD]:
            items = self.include_banks[bank]

            if len(items) == 0:
                continue

            sort_law = self._local_headers_sort_law if bank == IncludeType.LOCAL else lambda path: path.to_text()

            for item in sorted(items, key=sort_law):
                yield ('#include ' + ('"{0}"' if bank == IncludeType.LOCAL else '<{0}>')).format(item)

            yield ''

    def _local_headers_sort_law(self, path):
        if path == self.physical_path():
            return 0,  # corresponding header file always comes first
        else:
            return 1, path.to_text()

    def _gen_uses_lines(self):
        for bank in [UseType.STD, UseType.LOCAL]:
            items = self.use_banks[bank]

            for item in sorted(items, key=lambda path: path.to_text()):
                yield 'using namespace {0};'.format('::'.join(item.elements))

        yield ''

    def _gen_namespaced_lines(self):
        for line in self._gen_uses_lines():
            yield line

        for line in self.code_lines:
            yield line

    def _gen_guarded_lines(self):
        for line in self._gen_includes_lines():
            yield line

        yield ''
        namespace_decls = ['namespace {0} {{'.format(namespace) for namespace in self.namespace_path()]
        yield ' '.join(namespace_decls)
        yield ''

        for line in self._gen_namespaced_lines():
            yield line

        yield ''
        yield '}' * len(namespace_decls)

    def _gen_guards(self, guarded_lines):
        guard_name = '_'.join(self.namespace_path() + [self.name, 'h']).upper()

        yield '#ifndef ' + guard_name
        yield '#define ' + guard_name
        yield ''

        for line in guarded_lines:
            yield line

        yield ''
        yield '#endif //' + guard_name

    def _gen_body_lines(self):
        guarded_lines = self._gen_guarded_lines()

        for line in self._gen_guards(guarded_lines) if self.type == 'h' else guarded_lines:
            yield line

    def _gen_unfiltered_lines(self):
        for line in self._gen_header_lines():
            yield line

        yield ''

        for line in self._gen_body_lines():
            yield line

    def _gen_lines_without_hash(self):
        for line in filter_empty_lines(self._gen_unfiltered_lines()):
            yield line

    def _gen_lines(self):
        content_hash = compute_hash(self._gen_lines_without_hash())

        for line in self._gen_lines_without_hash():
            if line.startswith(HASH_LINE_PREFIX):
                yield line + ' ' + content_hash
            else:
                yield line


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
