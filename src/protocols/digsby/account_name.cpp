/**
 * protocols/digsby/account_name.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QRegularExpression>

#include "protocols/digsby/account_name.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace protocols { namespace digsby {

bool is_valid_digsby_account_name(IMM(QString) account_name) {
    static QRegularExpression pattern("^[a-z][a-z0-9_.]*(@digsby[.]org)?$", QRegularExpression::CaseInsensitiveOption);

    return pattern.match(account_name).hasMatch();
}

void assert_valid_digsby_account_name(IMM(QString) account_name) {
    invariant(
        is_valid_digsby_account_name(account_name),
        "'%s' doesn't look like a valid Digsby account name",
        QP(account_name)
    );
}

FullAccountName parse_digsby_account(IMM(QString) account_name) {
    assert_valid_digsby_account_name(account_name);
    return FullAccountName(IMProtocol::DIGSBY, account_name.section(QChar('@'), 0, 0));
}

}}}
