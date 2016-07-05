/**
 * intermediate_format/content/symbols/SkypeEmoticon.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/symbols/SkypeEmoticon.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

SkypeEmoticon::SkypeEmoticon(IMM(QString) semantics, IMM(QString) text_equivalent)
    : semantics(semantics), textEquivalent(text_equivalent) {
}

void SkypeEmoticon::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[SkypeEmoticon semantics=" << semantics << " text=" << textEquivalent << "]";
}

}}}
