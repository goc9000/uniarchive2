/**
 * protocols/phone/phone_number.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_PHONE_PHONE_NUMBER_H
#define UNIARCHIVE2_PROTOCOLS_PHONE_PHONE_NUMBER_H

#include "protocols/FullAccountName.h"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace protocols { namespace phone {

bool is_valid_looking_phone_number(IMM(QString) phone_number);
void assert_valid_looking_phone_number(IMM(QString) phone_number);
FullAccountName parse_phone_account(IMM(QString) phone_number);

}}}

#endif //UNIARCHIVE2_PROTOCOLS_PHONE_PHONE_NUMBER_H
