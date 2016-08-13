# build_assistant/VirtualPath.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import os


class VirtualPath:
    elements = None

    def __init__(self, elements):
        self.elements = list(elements)

    def basename(self):
        return self.elements[-1]

    def parent(self):
        return VirtualPath(self.elements[:-1])

    def depth(self):
        return len(self.elements)

    def add(self, element):
        return VirtualPath(self.elements + [element])

    def append(self, other_path):
        return VirtualPath(self.elements + other_path.elements)

    def relative_to(self, other_path):
        assert self.elements[:len(other_path.elements)] == other_path.elements

        return VirtualPath(self.elements[len(other_path.elements):])

    def realize(self):
        return os.path.join(*self.elements)

    def to_text(self):
        return '/'.join(self.elements)

    def __str__(self):
        return self.to_text()

    @staticmethod
    def from_text(path_text):
        return VirtualPath(elem for elem in path_text.split('/') if elem != '')