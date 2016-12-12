/**
 * intermediate_format/content/formatting/ItalicTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 539aa605b4b4db60dfc39803e3fb7ad8de68db96508af789353508ef85a1e8d6
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/ItalicTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

ItalicTag::ItalicTag(bool open) : StandardTag(open) {
}

RawMessageContentItemSubType ItalicTag::subType() const {
    return RawMessageContentItemSubType::ITALIC_TAG;
}

QString ItalicTag::tagName() const {
    return "I";
}

void ItalicTag::serializeOpenTagAttributesToStream(QDataStream& UNUSED mut_stream) const {
}

}}}
