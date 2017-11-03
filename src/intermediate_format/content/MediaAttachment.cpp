/**
 * intermediate_format/content/MediaAttachment.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 138e9bb6677831063af0a9d228c06eb722bd4fe3ff3aa0994eb1bce7f76687d3
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/MediaAttachment.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

MediaAttachment::MediaAttachment() : RawMessageContentItem() {
    // Nothing else to initialize
}

RawMessageContentItemSubType MediaAttachment::subType() const {
    return RawMessageContentItemSubType::MEDIA_ATTACHMENT;
}

bool MediaAttachment::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    // No fields to visit
    return true;
}

CEDE(MediaAttachment) MediaAttachment::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::MEDIA_ATTACHMENT);

    optional<QString> filename = must_deserialize(mut_stream, optional<QString>);
    optional<MediaError> error = must_deserialize(mut_stream, optional<MediaError>);

    unique_ptr<MediaAttachment> item = make_unique<MediaAttachment>();
    item->filename = filename;
    item->error = error;

    return item;
}

void MediaAttachment::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << filename << error;
}

void MediaAttachment::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[Media";
    if (filename) {
        stream << " filename=" << filename;
    }
    if (error) {
        stream << " error=" << error;
    }
    stream << "]";
}

}}}
