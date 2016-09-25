# build_assistant/util/FrozenStruct.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


class FrozenStruct:
    _fields = None
    _frozen = False

    def __init__(self, _fields, values):
        self._fields = _fields

        for field in values.keys():
            assert field in _fields, 'Extraneous field: {0}'.format(field)

        for field in _fields:
            assert field in values, 'Missing field: {0}'.format(field)
            setattr(self, field, values[field])

        self._frozen = True

    def __setattr__(self, key, value):
        assert not self._frozen, 'Object cannot be updated after freezing'
        object.__setattr__(self, key, value)

    def _replace(self, **kwargs):
        current_fields = {field: getattr(self, field) for field in self._fields}
        current_fields.update(kwargs)

        return type(self)(self._fields, **current_fields)
