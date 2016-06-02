/**
 * intermediate_format/subjects/FullySpecifiedSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

FullySpecifiedSubject::FullySpecifiedSubject(
    IMM(FullAccountName) account_name,
    IMM(QString) screen_name,
    Hints hints
) : ApparentSubject(hints), accountName(account_name), screenName(screen_name) {
}

CEDE(ApparentSubject) FullySpecifiedSubject::clone() const {
    return make_unique<FullySpecifiedSubject>(accountName, screenName, hints);
}

void FullySpecifiedSubject::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "Sbj:full:" << QP(screenName) << " [" << accountName << "]";
}

}}}
