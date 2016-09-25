# build_assistant/util/util.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import os
import sys

from textwrap import dedent

from build_assistant.util.VirtualPath import VirtualPath


def fail(format_str, *args, **kwargs):
    print(dedent(format_str.format(*args, **kwargs)).strip(), file=sys.stderr)
    sys.exit(-1)


def scan_files(base_dir, full_paths=True, include_dirs=False, filter=None):
    filter = filter or (lambda x: True)
    output = []

    def scan_rec(path):
        full_path = base_dir.append(path)
        if os.path.isdir(full_path.realize()):
            if include_dirs and filter(full_path):
                output.append(full_path if full_paths else path)

            for item in os.listdir(full_path.realize()):
                scan_rec(path.add(item))
        elif filter(full_path):
            output.append(full_path if full_paths else path)

    if not os.path.isdir(base_dir.realize()):
        raise FileNotFoundError(base_dir.realize())

    scan_rec(VirtualPath([]))

    return output
