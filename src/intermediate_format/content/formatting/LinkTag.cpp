/**
 * intermediate_format/content/formatting/LinkTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): d58dadca6514f516ec672771e922dca14c5c4f816858341a08de3241b42fa652
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/LinkTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

LinkTag::LinkTag(bool open) : StandardTag(open) {
}

LinkTag::LinkTag(IMM(QUrl) url) : StandardTag(true), url(url) {
}

RawMessageContentItemSubType LinkTag::subType() const {
    return RawMessageContentItemSubType::LINK_TAG;
}

QString LinkTag::tagName() const {
    return "Link";
}

void LinkTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    if (url) {
        stream << " " << url->url();
    }
}

}}}
