/**
 * protocols/phone/phone_number.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/phone/phone_number.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols { namespace phone {

bool is_valid_looking_phone_number(IMM(QString) phone_number) {
    QREGEX_MATCH_CI(match, "^[+]?[0-9]{4,}$", phone_number);
    return match.hasMatch();
}

void assert_valid_looking_phone_number(IMM(QString) phone_number) {
    invariant(
        is_valid_looking_phone_number(phone_number),
        "'%s' doesn't look like a valid phone number",
        QP(phone_number)
    );
}

FullAccountName parse_phone_account(IMM(QString) phone_number) {
    assert_valid_looking_phone_number(phone_number);
    return FullAccountName(IMProtocol::PHONE, phone_number);
}

}}}
