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
#include "utils/external_libs/make_unique.hpp"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

FullySpecifiedSubject::FullySpecifiedSubject(IMM(FullAccountName) account_name, IMM(QString) screen_name)
    : accountName(account_name), screenName(screen_name) {
}

CEDE(ApparentSubject) FullySpecifiedSubject::clone() const {
    return make_unique<FullySpecifiedSubject>(accountName, screenName);
}

void FullySpecifiedSubject::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "Sbj:full:" << qUtf8Printable(screenName) << " [" << accountName << "]";
}

}}}