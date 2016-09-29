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

#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/FullAccountName.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace uniarchive2::protocols;

class SubjectGivenAsAccount : public ApparentSubject {
public:
    FullAccountName account;

    SubjectGivenAsAccount(IMM(FullAccountName) account, Hints hints = Hint::NoHints);

    virtual CEDE(ApparentSubject) clone() const;

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_SUBJECTGIVENASACCOUNT_H
