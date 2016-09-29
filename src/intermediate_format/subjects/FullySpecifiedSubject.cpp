/**
 * intermediate_format/subjects/FullySpecifiedSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

FullySpecifiedSubject::FullySpecifiedSubject(
    IMM(FullAccountName) account_name,
    IMM(QString) screen_name,
    Hints hints
) : ApparentSubject(hints), accountName(account_name), screenName(screen_name) {
}

ApparentSubjectSubType FullySpecifiedSubject::subType() const {
    return ApparentSubjectSubType::FULLYSPECIFIEDSUBJECT;
}

CEDE(ApparentSubject) FullySpecifiedSubject::clone() const {
    return make_unique<FullySpecifiedSubject>(accountName, screenName, hints);
}

void FullySpecifiedSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:full:" << QP(screenName) << " [" << accountName << "]";
}

}}}
