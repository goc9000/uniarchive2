/**
 * protocols/parse_account_generic.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "utils/language/invariant.h"
#include "protocols/digsby/account_name.h"
#include "protocols/jabber/account_name.h"
#include "protocols/msn/account_name.h"
#include "protocols/yahoo/account_name.h"
#include "protocols/IMProtocol.h"

using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::protocols::jabber;
using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::protocols::yahoo;

namespace uniarchive2 { namespace protocols {

FullAccountName parse_account_generic(IMProtocol protocol, IMM(QString) account_name) {
    switch (protocol) {
        case IMProtocol::DIGSBY: return parse_digsby_account(account_name);
        case IMProtocol::JABBER: return parse_jabber_account(account_name);
        case IMProtocol::MSN: return parse_msn_account(account_name);
        case IMProtocol::YAHOO: return parse_yahoo_account(account_name);
        default:
            invariant_violation(
                "Unsupported protocol for parsing an account: %s",
                qUtf8Printable(name_for_im_protocol(protocol))
            );
    }
}

}}
