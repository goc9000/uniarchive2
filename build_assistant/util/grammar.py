# build_assistant/util/grammar.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import re


def camelcase_to_underscore(name):
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)).lower()


def classname_to_varname(name):
    return camelcase_to_underscore(name).split('_')[-1]


def singular(word):
    if word.endswith('s'):
        return word[:-1]

    assert False, "Don't know how to produce singular of '{0}'".format(word)
