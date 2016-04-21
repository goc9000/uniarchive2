/**
 * intermediate_format/subjects/SubjectGivenAsAccount.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASACCOUNT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASACCOUNT_H

#include <memory>

#include <QtDebug>

#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/FullAccountName.h"

using namespace std;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class SubjectGivenAsAccount : public ApparentSubject {
public:
    FullAccountName account;

    SubjectGivenAsAccount(FullAccountName account);

    virtual unique_ptr<ApparentSubject> clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASACCOUNT_H
