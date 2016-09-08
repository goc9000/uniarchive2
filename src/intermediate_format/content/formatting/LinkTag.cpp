/**
 * intermediate_format/content/formatting/LinkTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/LinkTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

LinkTag::LinkTag(QUrl url) : StandardTag(true), url(url) {
}

LinkTag::LinkTag(bool open) : StandardTag(open) {
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
