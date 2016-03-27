/**
 * protocols/yahoo/utils.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_YAHOO_UTILS_H
#define UNIARCHIVE2_PROTOCOLS_YAHOO_UTILS_H

#include <QString>

namespace uniarchive2 { namespace protocols { namespace yahoo {

bool is_valid_yahoo_account_name(const QString& account_name);

}}}

#endif //UNIARCHIVE2_PROTOCOLS_YAHOO_UTILS_H
