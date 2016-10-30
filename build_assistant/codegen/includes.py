# build_assistant/codegen/includes.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


from enum import Enum

from build_assistant.util.VirtualPath import VirtualPath


IncludeType = Enum('IncludeType', ['LOCAL', 'STD', 'QT'])
UseType = Enum('IncludeType', ['LOCAL', 'STD'])


def normalize_include_path(path):
    if isinstance(path, VirtualPath):
        return path
    elif isinstance(path, str):
        return VirtualPath.from_text(path)
    else:
        assert False, "Include path should be VirtualPath or string, is {0}".format(path)


def normalize_use_path(path):
    if isinstance(path, VirtualPath):
        return path
    elif isinstance(path, str):
        if '::' in path:
            return VirtualPath(path.split('::'))

        return VirtualPath.from_text(path)
    else:
        assert False, "Use path should be VirtualPath or string, is {0}".format(path)


def local_include(path):
    return IncludeType.LOCAL, normalize_include_path(path)


def std_include(path):
    return IncludeType.STD, normalize_include_path(path)


def qt_include(path):
    return IncludeType.QT, normalize_include_path(path)


def local_use(path):
    return UseType.LOCAL, normalize_use_path(path)


def std_use(path):
    return UseType.STD, normalize_use_path(path)
