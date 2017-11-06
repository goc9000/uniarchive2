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
#include "protocols/facebook/facebook_account_name.h"
#include "protocols/jabber/jabber_account_name.h"
#include "protocols/msn/msn_account_name.h"
#include "protocols/phone/phone_number.h"
#include "protocols/skype/skype_account_name.h"
#include "protocols/yahoo/yahoo_account_name.h"
#include "protocols/IMProtocol.h"
#include "utils/json/json_utils.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols {

using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::protocols::facebook;
using namespace uniarchive2::protocols::jabber;
using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::protocols::phone;
using namespace uniarchive2::protocols::skype;
using namespace uniarchive2::protocols::yahoo;

using namespace uniarchive2::utils::json;

FullAccountName parse_account_generic(IMProtocol protocol, IMM(QString) account_name) {
    switch (protocol) {
        case IMProtocol::DIGSBY: return parse_digsby_account(account_name);
        case IMProtocol::FACEBOOK: return parse_facebook_account(account_name);
        case IMProtocol::JABBER: return parse_jabber_account(account_name);
        case IMProtocol::MSN: return parse_msn_account(account_name);
        case IMProtocol::SKYPE: return parse_skype_account(account_name);
        case IMProtocol::YAHOO: return parse_yahoo_account(account_name);
        case IMProtocol::PHONE: return parse_phone_account(account_name);
        default:
            invariant_violation("Unsupported protocol for parsing an account: %s", QP(name_for_im_protocol(protocol)));
    }
}

FullAccountName parse_account_from_json(IMM(QJsonValue) json) {
    QJsonArray as_array = expect_json_array(json);
    invariant(as_array.size() == 2, "Accounts should be represented by a JSON array of exactly 2 elements");

    return parse_account_generic(
        im_protocol_from_symbol(expect_json_string(as_array[0])),
        expect_json_string(as_array[1])
    );
}

}}
