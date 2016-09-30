/**
 * intermediate_format/subjects/AccountSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/AccountSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

AccountSubject::AccountSubject(IMM(FullAccountName) account, Hints hints) : ApparentSubject(hints), account(account) {
}

ApparentSubjectSubType AccountSubject::subType() const {
    return ApparentSubjectSubType::ACCOUNT;
}

CEDE(ApparentSubject) AccountSubject::clone() const {
    return make_unique<AccountSubject>(account, hints);
}

void AccountSubject::serializeToStreamSubImpl(QDataStream &mut_stream) const {
    mut_stream << account;
}

void AccountSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:acct:" << account;
}

}}}
