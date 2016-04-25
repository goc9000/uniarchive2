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

#include <memory>

#include <QtDebug>
#include <QString>

#include "intermediate_format/subjects/ApparentSubject.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class SubjectGivenAsScreenName : public ApparentSubject {
public:
    QString screenName;

    SubjectGivenAsScreenName(const QString& screen_name);

    virtual unique_ptr<ApparentSubject> clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASSCREENNAME_H
