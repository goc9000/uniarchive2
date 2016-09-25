# build_assistant/util/Augment.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


class Augment:
    _augmented_object = None

    def __init__(self, augmented_object, expected_type):
        assert isinstance(augmented_object, expected_type), \
            'Augmented object should be {0}'.format(expected_type.__name__)

        self._augmented_object = augmented_object

    def __getattr__(self, name):
        return getattr(self._augmented_object, name)

    def _replace(self, **_):
        assert False, 'Augments do not support _replace'
