/**
 * protocols/msn/account_name.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_MSN_ACCOUNT_NAME_H
#define UNIARCHIVE2_PROTOCOLS_MSN_ACCOUNT_NAME_H

#include <QString>

#include "protocols/FullAccountName.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace protocols { namespace msn {

bool is_valid_msn_account_name(IMM(QString) account_name);
void assert_valid_msn_account_name(IMM(QString) account_name);
FullAccountName parse_msn_account(IMM(QString) account_name);

bool is_likely_valid_encoded_msn_account_name(IMM(QString) account_name);
void assert_likely_valid_encoded_msn_account_name(IMM(QString) account_name);
FullAccountName parse_encoded_msn_account(IMM(QString) account_name);

bool is_likely_valid_optionally_encoded_msn_account_name(IMM(QString) account_name);
void assert_likely_valid_optionally_encoded_msn_account_name(IMM(QString) account_name);
FullAccountName parse_optionally_encoded_msn_account(IMM(QString) account_name);

}}}

#endif //UNIARCHIVE2_PROTOCOLS_MSN_ACCOUNT_NAME_H
