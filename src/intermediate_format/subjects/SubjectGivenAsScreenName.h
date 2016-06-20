/**
 * intermediate_format/subjects/SubjectGivenAsScreenName.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASSCREENNAME_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASSCREENNAME_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

#include <memory>

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace std;

class SubjectGivenAsScreenName : public ApparentSubject {
public:
    QString screenName;

    SubjectGivenAsScreenName(IMM(QString) screen_name, Hints hints = Hint::NoHints);

    virtual CEDE(ApparentSubject) clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASSCREENNAME_H
