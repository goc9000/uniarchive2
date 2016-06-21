/**
 * protocols/parse_account_generic.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_PARSE_ACCOUNT_GENERIC_H
#define UNIARCHIVE2_PROTOCOLS_PARSE_ACCOUNT_GENERIC_H

#include "utils/language/shortcuts.h"
#include "protocols/IMProtocol.h"
#include "protocols/FullAccountName.h"

#include <QString>

namespace uniarchive2 { namespace protocols {

FullAccountName parse_account_generic(IMProtocol protocol, IMM(QString) account_name);

}}

#endif //UNIARCHIVE2_PROTOCOLS_PARSE_ACCOUNT_GENERIC_H
