#!/usr/bin/env python3

# build_assistant.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import argparse
import os
import re
import sys
import yaml

from collections import namedtuple

from build_assistant.VirtualPath import VirtualPath
from build_assistant.AutoGeneratedSource import CodeGenConfig
from build_assistant.AutoGenConfig import parse_autogen_config
from build_assistant.AutoGenCore import AutoGenCore
from build_assistant.SymbolRegistry import SymbolRegistry
from build_assistant.autogen_enums import gen_enums
from build_assistant.autogen_raw_events import gen_raw_events
from build_assistant.util import fail, scan_files


AppMetadata = namedtuple('AppMetadata', ['app_name', 'copyright_text', 'license_text'])


APP_METADATA = AppMetadata(
    app_name='UniArchive II',
    copyright_text='(C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>',
    license_text='Licensed under the GPL-3',
)

CODE_GEN_CFG = CodeGenConfig(
    app_metadata=APP_METADATA,
    indent_size=4,
    gutter_width=120,
    base_namespace='uniarchive2',
)

BASE_SRC_DIR = VirtualPath.from_text('src')


def autogenerate_code():
    with open('autogen_config.yml', 'r') as f:
        raw_config = yaml.load(f)

    autogen_config = parse_autogen_config(raw_config)
    symbol_registry = SymbolRegistry.build(BASE_SRC_DIR, autogen_config, CODE_GEN_CFG)

    autogen_core = AutoGenCore(autogen_config, symbol_registry, CODE_GEN_CFG)

    gen_enums(autogen_config.enums, autogen_core)
    gen_raw_events(autogen_config, autogen_core)

    autogen_core.commit(BASE_SRC_DIR)


def cleanup_pro_file():
    def keyfn_order_directories_first(path):
        return [' ' + element for element in path.elements[:-1]] + [path.elements[-1]]

    real_path = BASE_SRC_DIR.add('uniarchive2.pro').realize()

    with open(real_path, 'rt') as f:
        contents = f.read()

    if not contents.endswith("\n"):
        contents += "\n"

    has_effect = False

    for item_name, varname, extensions in [
        ('source', 'SOURCES', ['.cpp', '.c']),
        ('header', 'HEADERS', ['.h', '.hpp'])
    ]:
        pattern = re.compile("(?<=\n)" + varname + "\s*[+]=\s*((?:[^\n\\\\]*\\\\\n)+[^\n]*\n)")

        match = pattern.search(contents)
        assert match is not None, 'Could not find ' + varname + ' declaration in makefile'

        declared_paths = re.split("\\s+", pattern.search(contents).group(1).replace("\\\n", " ").strip())

        actual_files = scan_files(BASE_SRC_DIR, full_paths=False, filter=lambda p: p.extension().lower() in extensions)
        for file in actual_files:
            if ' ' in file.to_text():
                fail("File '{0}' contains spaces, these are not supported by the Qt infrastructure", file.realize())

        actual_paths = [f.realize() for f in sorted(actual_files, key=keyfn_order_directories_first)]

        for item in sorted(set(actual_paths) - set(declared_paths)):
            print("Added {0}: {1}".format(item_name, item))
        for item in sorted(set(declared_paths) - set(actual_paths)):
            print("Removed {0}: {1}".format(item_name, item))

        if actual_paths != declared_paths:
            has_effect = True
            print("Reordered {0}".format(varname))

        contents = pattern.sub(" \\\n    ".join([varname + " +="] + actual_paths) + "\n", contents)

    if has_effect:
        with open(real_path, 'wt') as f:
            f.write(contents)


def sanity_check():
    if not os.path.isdir(BASE_SRC_DIR.realize()):
        fail(
            "Directory '{0}' was not found. Check that you are running the script in the right location.",
            BASE_SRC_DIR.to_text()
        )


def main():
    parser = argparse.ArgumentParser(description='Build assistant for Uniarchive II')

    subparsers = parser.add_subparsers(title='commands', dest='command')

    subparsers.add_parser('gencode', help='Build autogenerated code')
    subparsers.add_parser('cleanpro', help='Cleanup source list in QT .pro file')

    raw_args = parser.parse_args(sys.argv[1:])

    if raw_args.command is None:
        parser.print_usage()
        return

    sanity_check()

    if raw_args.command == 'gencode':
        autogenerate_code()
    elif raw_args.command == 'cleanpro':
        cleanup_pro_file()

main()
