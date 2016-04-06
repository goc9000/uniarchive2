/**
 * protocols/yahoo/account_name.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_YAHOO_ACCOUNT_NAME_H
#define UNIARCHIVE2_PROTOCOLS_YAHOO_ACCOUNT_NAME_H

#include <QString>

#include "protocols/FullAccountName.h"

namespace uniarchive2 { namespace protocols { namespace yahoo {

bool is_valid_yahoo_account_name(const QString& account_name);
void assert_valid_yahoo_account_name(const QString& account_name);
FullAccountName parse_yahoo_account(const QString& account_name);

}}}

#endif //UNIARCHIVE2_PROTOCOLS_YAHOO_ACCOUNT_NAME_H
