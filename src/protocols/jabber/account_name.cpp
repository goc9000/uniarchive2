/**
 * protocols/jabber/account_name.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "protocols/jabber/account_name.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace protocols { namespace jabber {

bool is_valid_jabber_account_name(IMM(QString) account_name) {
    QREGEX_MATCH_CI(match, "^[a-z0-9_.]+@[a-z][a-z0-9_]*([.][a-z][a-z0-9_]*)*$", account_name);
    return match.hasMatch();
}

void assert_valid_jabber_account_name(IMM(QString) account_name) {
    invariant(
        is_valid_jabber_account_name(account_name),
        "'%s' doesn't look like a valid GTalk/Jabber account name",
        QP(account_name)
    );
}

FullAccountName parse_jabber_account(IMM(QString) account_name) {
    assert_valid_jabber_account_name(account_name);
    return FullAccountName(IMProtocol::JABBER, account_name);
}

}}}
