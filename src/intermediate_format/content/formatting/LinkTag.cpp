/**
 * intermediate_format/content/formatting/LinkTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 1ce7ccf36ea4a7e9a368eb47b60c631aced2f2da8c70125c0b4cc17e7f7d5e88
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/LinkTag.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

LinkTag::LinkTag(bool open) : StandardTag(open) {
    // Nothing else to initialize
}

LinkTag::LinkTag(IMM(QUrl) url) : StandardTag(true), url(url) {
    // Nothing else to initialize
}

RawMessageContentItemSubType LinkTag::subType() const {
    return RawMessageContentItemSubType::LINK_TAG;
}

QString LinkTag::tagName() const {
    return "Link";
}

bool LinkTag::visitSubjectsImpl(IMM(visit_subjects_callback_t) UNUSED callback) {
    // No fields to visit
    return true;
}

CEDE(LinkTag) LinkTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::LINK_TAG);

    if (!must_deserialize(mut_stream, bool)) {
        return make_unique<LinkTag>(false);
    }

    optional<QUrl> url = must_deserialize(mut_stream, optional<QUrl>);
    optional<RawSharedURLInfo> target_info = must_deserialize(mut_stream, optional<RawSharedURLInfo>);

    unique_ptr<LinkTag> item = make_unique<LinkTag>(true);
    item->url = url;
    item->targetInfo = target_info;

    return item;
}

void LinkTag::serializeOpenTagAttributesToStream(QDataStream& mut_stream) const {
    mut_stream << url << targetInfo;
}

void LinkTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    if (url) {
        stream << " " << url->url();
    }
    if (targetInfo) {
        stream << " target_info=" << targetInfo;
    }
}

}}}
