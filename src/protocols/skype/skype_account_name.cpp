/**
 * protocols/skype/skype_account_name.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/skype/skype_account_name.h"
#include "protocols/phone/phone_number.h"
#include "protocols/msn/msn_account_name.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace protocols { namespace skype {

using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::protocols::phone;

bool is_valid_pure_skype_account_name(IMM(QString) account_name) {
    QREGEX_MATCH_CI(match, "^[a-z0-9._-]+$", account_name);
    return match.hasMatch();
}

void assert_valid_pure_skype_account_name(IMM(QString) account_name) {
    invariant(
        is_valid_pure_skype_account_name(account_name),
        "'%s' doesn't look like a valid Skype account name",
        QP(account_name)
    );
}

FullAccountName parse_pure_skype_account(IMM(QString) account_name) {
    assert_valid_pure_skype_account_name(account_name);
    return FullAccountName(IMProtocol::SKYPE, account_name);
}

bool is_valid_skype_account_name(IMM(QString) account_name) {
    if (account_name.startsWith("1:")) {
        return is_valid_msn_account_name(account_name.mid(2));
    }

    return is_valid_looking_phone_number(account_name) || is_valid_pure_skype_account_name(account_name);
}

void assert_valid_skype_account_name(IMM(QString) account_name) {
    invariant(
        is_valid_skype_account_name(account_name),
        "'%s' doesn't look like a valid Skype account name",
        QP(account_name)
    );
}

FullAccountName parse_skype_account(IMM(QString) account_name) {
    assert_valid_skype_account_name(account_name);

    if (account_name.startsWith("1:")) {
        return parse_msn_account(account_name.mid(2));
    }

    return is_valid_looking_phone_number(account_name)
        ? parse_phone_account(account_name) : parse_pure_skype_account(account_name);
}

}}}

