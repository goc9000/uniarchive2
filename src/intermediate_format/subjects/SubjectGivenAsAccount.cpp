/**
 * intermediate_format/subjects/SubjectGivenAsAccount.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/SubjectGivenAsAccount.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

SubjectGivenAsAccount::SubjectGivenAsAccount(IMM(FullAccountName) account, Hints hints)
    : ApparentSubject(hints), account(account) {
}

ApparentSubjectSubType SubjectGivenAsAccount::subType() const {
    return ApparentSubjectSubType::ACCOUNT;
}

CEDE(ApparentSubject) SubjectGivenAsAccount::clone() const {
    return make_unique<SubjectGivenAsAccount>(account, hints);
}

void SubjectGivenAsAccount::serializeToStreamSubImpl(QDataStream &mut_stream) const {
    mut_stream << account;
}

void SubjectGivenAsAccount::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:acct:" << account;
}

}}}
