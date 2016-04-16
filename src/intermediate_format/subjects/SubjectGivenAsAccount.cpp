/**
 * intermediate_format/subjects/SubjectGivenAsAccount.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/subjects/SubjectGivenAsAccount.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

SubjectGivenAsAccount::SubjectGivenAsAccount(FullAccountName account) : account(account) {
}

SubjectGivenAsAccount* SubjectGivenAsAccount::clone() const {
    return new SubjectGivenAsAccount(account);
}

void SubjectGivenAsAccount::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "Sbj:acct:" << account;
}

}}}
