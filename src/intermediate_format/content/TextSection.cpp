/**
 * intermediate_format/content/TextSection.cpp
 *
 * WARNING: This file is partially autogenerated. Edit only the custom sections, or autogen_config.yml and re-generate.
 * Codegen hash (sha256): e06408ee51a556776e29afd108de5810a5810be6aa3ea559d3d4517d7cd7ffb7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/TextSection.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

TextSection::TextSection(IMM(QString) text) : RawMessageContentItem(), text(text) {
    // Nothing else to initialize
}

RawMessageContentItemSubType TextSection::subType() const {
    return RawMessageContentItemSubType::TEXT_SECTION;
}

bool TextSection::visitSubjectsImpl(IMM(visit_subjects_callback_t) UNUSED callback) {
    // No fields to visit
    return true;
}

CEDE(TextSection) TextSection::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::TEXT_SECTION);

    QString text = must_deserialize(mut_stream, QString);

    unique_ptr<TextSection> item = make_unique<TextSection>(text);

    return item;
}

void TextSection::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << text;
}

void TextSection::writeToDebugStreamImpl(QDebug stream) const {
    // BEGIN CUSTOM SECTION: Debug write method
    stream << text;
    // END CUSTOM SECTION: Debug write method
}

}}}
