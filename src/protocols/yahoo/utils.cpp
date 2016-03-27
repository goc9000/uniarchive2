/**
 * protocols/yahoo/utils.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QRegularExpression>

#include "protocols/yahoo/utils.h"

namespace uniarchive2 { namespace protocols { namespace yahoo {

bool is_valid_yahoo_account_name(const QString& account_name) {
    static QRegularExpression pattern("[a-z][a-z0-9_.]*", QRegularExpression::CaseInsensitiveOption);

    return pattern.match(account_name).hasMatch();
}

}}}

