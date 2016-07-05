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

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

LinkTag::LinkTag(QUrl url) : url(url), closed(false) {
}

LinkTag::LinkTag(bool closed) : closed(closed) {
}

void LinkTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Link";
    if (!url.isEmpty()) {
        stream << " url=" << url.toString();
    }
    stream << "]";
}

}}}
