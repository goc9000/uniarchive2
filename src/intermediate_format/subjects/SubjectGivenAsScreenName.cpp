/**
 * intermediate_format/subjects/SubjectGivenAsScreenName.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "utils/external_libs/make_unique.hpp"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

SubjectGivenAsScreenName::SubjectGivenAsScreenName(IMM(QString) screen_name) : screenName(screen_name) {
}

CEDE(ApparentSubject) SubjectGivenAsScreenName::clone() const {
    return make_unique<SubjectGivenAsScreenName>(screenName);
}

void SubjectGivenAsScreenName::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "Sbj:scrname:" << qUtf8Printable(screenName);
}

}}}
