# build_assistant/AutoGenCore.py
#
# (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
#
# This file is part of Uniarchive II.
#
# Licensed under the GPL-3


import os

from build_assistant.AutoGeneratedSource import AutoGeneratedSource
from build_assistant.autogen_common import get_hashes_and_custom_sections
from build_assistant.util.util import fail


class AutoGenCore:
    autogen_config = None
    symbol_registry = None
    codegen_cfg = None

    sources = None

    def __init__(self, autogen_config, symbol_registry, code_gen_config):
        self.autogen_config = autogen_config
        self.symbol_registry = symbol_registry
        self.codegen_cfg = code_gen_config

        self.sources = dict()

    def new_h(self, base_path, name):
        return self._new(base_path, name, 'h')

    def new_cpp(self, base_path, name, companion=None):
        return self._new(base_path, name, 'cpp', companion=companion)

    def new_pair(self, base_path, name):
        h_source = self.new_h(base_path, name)
        cpp_source = self.new_cpp(base_path, name, companion=h_source)

        return cpp_source, h_source

    def _new(self, base_path, name, type, companion=None):
        full_path = base_path.add(name + '.' + type)

        assert full_path not in self.sources, 'Duplicate source {0}'.format(full_path.to_text())

        source = AutoGeneratedSource(base_path, name, type, self, companion=companion)
        self.sources[full_path] = source

        return source

    def commit(self, base_src_dir):
        for source in self.sources.values():
            real_path = base_src_dir.append(source.physical_path()).realize()

            os.makedirs(base_src_dir.append(source.physical_path().parent()).realize(), exist_ok=True)

            if os.path.isfile(real_path):
                # Check already existing file
                with open(real_path, 'rt') as f:
                    trim_nl = lambda line: line[:-1] if line.endswith("\n") else line
                    actual_hash, declared_hash, sections = get_hashes_and_custom_sections(map(trim_nl, f.readlines()))

                if declared_hash is None:
                    fail(
                        "No line specifying the content hash found for {0}. It might not be an autogenerated file!\n"
                        "Please double-check the autogeneration config. Aborting so as not to accidentally overwrite "
                        "manual files.",
                        real_path
                    )
                if actual_hash != declared_hash:
                    fail(
                        "Content hash is incorrect for {0}. The file has been modified manually.\n"
                        "Please save any manual changes so they can be applied elsewhere, and delete the file so it "
                         "can be regenerated.",
                        real_path
                    )
                new_sections = source.get_custom_sections()
                for section in sections.keys():
                    if not section in new_sections:
                        fail(
                            "Custom section '{0}' is no longer generated for {1}.\n"
                            "Please save any manual changes in the section and delete it.",
                            section,
                            real_path
                        )
            else:
                sections = dict()

            with open(real_path, 'wt') as f:
                f.write(source.generated_content(sections))
