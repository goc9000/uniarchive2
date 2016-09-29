/**
 * intermediate_format/subjects/SubjectGivenAsScreenName.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

SubjectGivenAsScreenName::SubjectGivenAsScreenName(
    IMM(QString) screen_name,
    Hints hints
) : ApparentSubject(hints), screenName(screen_name) {
}

ApparentSubjectSubType SubjectGivenAsScreenName::subType() const {
    return ApparentSubjectSubType::SUBJECTGIVENASSCREENNAME;
}

CEDE(ApparentSubject) SubjectGivenAsScreenName::clone() const {
    return make_unique<SubjectGivenAsScreenName>(screenName, hints);
}

void SubjectGivenAsScreenName::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:scrname:" << QP(screenName);
}

}}}
