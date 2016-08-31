# build_assistant/autogen_common.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3

import hashlib


HASH_LINE_PREFIX = ' * Codegen hash (sha256):'
BEGIN_CUSTOM_SECTION_LINE_PREFIX = '// BEGIN CUSTOM SECTION: '
END_CUSTOM_SECTION_LINE_PREFIX = '// END CUSTOM SECTION: '


def compute_content_hash(lines):
    actual_hash, _, _ = get_hashes_and_custom_sections(lines)

    return actual_hash


def get_hashes_and_custom_sections(lines):
    declared_hash = None
    sections = dict()
    in_section = None

    hasher = hashlib.sha256()

    for line in lines:
        if line.startswith(HASH_LINE_PREFIX):
            if len(line) > len(HASH_LINE_PREFIX):
                declared_hash = line[len(HASH_LINE_PREFIX) + 1:]
        elif line.strip().startswith(BEGIN_CUSTOM_SECTION_LINE_PREFIX):
            assert in_section is None, 'Opening custom section when already in section "{0}"'.format(in_section)
            in_section = line.strip()[len(BEGIN_CUSTOM_SECTION_LINE_PREFIX):]
            assert in_section != '', 'Section name must not be empty'
            assert in_section not in sections, 'Custom section "{0}" opened more than once'.format(in_section)
            sections[in_section] = list()
        elif line.strip().startswith(END_CUSTOM_SECTION_LINE_PREFIX):
            name = line.strip()[len(END_CUSTOM_SECTION_LINE_PREFIX):]
            assert name == in_section, 'Closing section "{0}", but open section is: "{1}"'.format(name, in_section)
            in_section = None
            hasher.update(line.encode('ascii'))
        elif in_section is None:
            hasher.update(line.encode('ascii'))
        else:
            sections[in_section].append(line)

    actual_hash = hasher.hexdigest()

    return actual_hash, declared_hash, sections
