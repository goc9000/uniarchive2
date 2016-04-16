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

#include <QtDebug>

#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/FullAccountName.h"

using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

struct SubjectGivenAsAccount : ApparentSubject {
public:
    FullAccountName account;

    SubjectGivenAsAccount(FullAccountName account);

    virtual SubjectGivenAsAccount* clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASACCOUNT_H
