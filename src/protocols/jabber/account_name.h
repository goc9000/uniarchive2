/**
 * protocols/jabber/account_name.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_JABBER_ACCOUNT_NAME_H
#define UNIARCHIVE2_PROTOCOLS_JABBER_ACCOUNT_NAME_H

#include <QString>

#include "protocols/FullAccountName.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace protocols { namespace jabber {

bool is_valid_jabber_account_name(IMM(QString) account_name);
void assert_valid_jabber_account_name(IMM(QString) account_name);
FullAccountName parse_jabber_account(IMM(QString) account_name);

}}}

#endif //UNIARCHIVE2_PROTOCOLS_JABBER_ACCOUNT_NAME_H
