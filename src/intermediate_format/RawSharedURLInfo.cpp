/**
 * intermediate_format/RawSharedURLInfo.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/RawSharedURLInfo.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format {

RawSharedURLInfo::RawSharedURLInfo() {
    // Nothing else to initialize
}

RawSharedURLInfo RawSharedURLInfo::deserializeFromStream(QDataStream& mut_stream) {
    RawSharedURLInfo url_info;

    url_info.title = must_deserialize(mut_stream, optional<QString>);
    url_info.description = must_deserialize(mut_stream, optional<QString>);
    url_info.siteName = must_deserialize(mut_stream, optional<QString>);
    url_info.mimeType = must_deserialize(mut_stream, optional<QString>);
    url_info.typeHint = must_deserialize(mut_stream, optional<URLTargetTypeHint>);

    return url_info;
}

void RawSharedURLInfo::serializeToStream(QDataStream& mut_stream) const {
    mut_stream << title << description << siteName << mimeType << typeHint;
}

QDebug operator<< (QDebug stream, IMM(RawSharedURLInfo) url_info) {
    QDebugStateSaver settings(stream);
    stream.nospace() << "[URLInfo";
    if (url_info.title) {
        stream << " title=" << *url_info.title;
    }
    if (url_info.description) {
        stream << " description=" << *url_info.description;
    }
    if (url_info.siteName) {
        stream << " site_name=" << *url_info.siteName;
    }
    if (url_info.mimeType) {
        stream << " mime_type=" << *url_info.mimeType;
    }
    if (url_info.typeHint) {
        stream << " type_hint=" << *url_info.typeHint;
    }
    stream << "]";

    return stream;
}

}}
