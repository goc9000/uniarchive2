/**
 * protocols/parse_account_generic.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/parse_account_generic.h"
#include "protocols/digsby/digsby_account_name.h"
#include "protocols/jabber/jabber_account_name.h"
#include "protocols/msn/msn_account_name.h"
#include "protocols/yahoo/yahoo_account_name.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols {

using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::protocols::jabber;
using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::protocols::yahoo;

FullAccountName parse_account_generic(IMProtocol protocol, IMM(QString) account_name) {
    switch (protocol) {
        case IMProtocol::DIGSBY: return parse_digsby_account(account_name);
        case IMProtocol::JABBER: return parse_jabber_account(account_name);
        case IMProtocol::MSN: return parse_msn_account(account_name);
        case IMProtocol::YAHOO: return parse_yahoo_account(account_name);
        default:
            invariant_violation("Unsupported protocol for parsing an account: %s", QP(name_for_im_protocol(protocol)));
    }
}

}}
