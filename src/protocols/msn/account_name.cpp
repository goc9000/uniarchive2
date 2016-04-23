/**
 * protocols/msn/account_name.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QRegularExpression>

#include "utils/language/invariant.h"
#include "protocols/msn/account_name.h"
#include "protocols/IMProtocols.h"

namespace uniarchive2 { namespace protocols { namespace msn {

bool is_valid_msn_account_name(const QString& account_name) {
    // Note: this is not completely accurate, but good enough for a quick check
    static QRegularExpression pattern(
        "^[a-z][a-z0-9_.-]*@(live|hotmail|outlook)[.]com$",
        QRegularExpression::CaseInsensitiveOption
    );

    return pattern.match(account_name).hasMatch();
}

void assert_valid_msn_account_name(const QString& account_name) {
    invariant(
        is_valid_msn_account_name(account_name),
        "'%s' doesn't look like a valid MSN account name",
        qUtf8Printable(account_name)
    );
}

FullAccountName parse_msn_account(const QString& account_name) {
    assert_valid_msn_account_name(account_name);
    return FullAccountName(IMProtocols::MSN, account_name);
}

bool is_likely_valid_encoded_msn_account_name(const QString& account_name) {
    // We say "likely" valid because we don't check the validity of the hash (it's expensive)
    static QRegularExpression pattern(
        "^[a-z][a-z0-9_.-]*?[0-9]+$",
        QRegularExpression::CaseInsensitiveOption
    );

    return pattern.match(account_name).hasMatch();
}

void assert_likely_valid_encoded_msn_account_name(const QString& account_name) {
    invariant(
        is_likely_valid_encoded_msn_account_name(account_name),
        "'%s' doesn't look like a valid encoded MSN account name",
        qUtf8Printable(account_name)
    );
}

bool is_likely_valid_optionally_encoded_msn_account_name(const QString& account_name) {
    return is_valid_msn_account_name(account_name) || is_likely_valid_encoded_msn_account_name(account_name);
}

void assert_likely_valid_optionally_encoded_msn_account_name(const QString& account_name) {
    invariant(
        is_likely_valid_optionally_encoded_msn_account_name(account_name),
        "'%s' doesn't look like a valid (optionally encoded) MSN account name",
        qUtf8Printable(account_name)
    );
}

}}}
