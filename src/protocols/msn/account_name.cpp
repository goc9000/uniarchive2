/**
 * protocols/msn/account_name.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QRegularExpression>
#include <QStringList>

#include "utils/language/invariant.h"
#include "protocols/msn/account_name.h"
#include "protocols/IMProtocol.h"

namespace uniarchive2 { namespace protocols { namespace msn {

bool is_valid_msn_account_name(IMM(QString) account_name) {
    // Note: this is not completely accurate, but good enough for a quick check
    static QRegularExpression pattern(
        "^[a-z][a-z0-9_.-]*@(live|hotmail|outlook)[.]com$",
        QRegularExpression::CaseInsensitiveOption
    );

    return pattern.match(account_name).hasMatch();
}

void assert_valid_msn_account_name(IMM(QString) account_name) {
    invariant(
        is_valid_msn_account_name(account_name),
        "'%s' doesn't look like a valid MSN account name",
        qUtf8Printable(account_name)
    );
}

FullAccountName parse_msn_account(IMM(QString) account_name) {
    assert_valid_msn_account_name(account_name);
    return FullAccountName(IMProtocol::MSN, account_name);
}

bool is_likely_valid_encoded_msn_account_name(IMM(QString) account_name) {
    // We say "likely" valid because we don't check the validity of the hash (it's expensive)
    static QRegularExpression pattern(
        "^[a-z][a-z0-9_.-]*?[0-9]+$",
        QRegularExpression::CaseInsensitiveOption
    );

    return pattern.match(account_name).hasMatch();
}

void assert_likely_valid_encoded_msn_account_name(IMM(QString) account_name) {
    invariant(
        is_likely_valid_encoded_msn_account_name(account_name),
        "'%s' doesn't look like a valid encoded MSN account name",
        qUtf8Printable(account_name)
    );
}

quint32 update_messenger_hash(quint32 old_hash, QChar character) {
    return (old_hash * 101) + character.unicode();
}

FullAccountName parse_encoded_msn_account(IMM(QString) account_name) {
    assert_likely_valid_encoded_msn_account_name(account_name);

    static QRegularExpression pattern("^([a-z][a-z0-9_.-]*?)[0-9]+$", QRegularExpression::CaseInsensitiveOption);

    unsigned int assumed_length = (unsigned int)pattern.match(account_name).capturedLength(1);
    unsigned int full_length = (unsigned int)account_name.length();

    quint32 base_hash = 0;
    for (unsigned int i = 0; i < assumed_length; i++) {
        base_hash = update_messenger_hash(base_hash, account_name[i]);
    }

    for (; assumed_length < full_length; base_hash = update_messenger_hash(base_hash, account_name[assumed_length++])) {
        unsigned int expected_hash_length = full_length - assumed_length;
        if (expected_hash_length > 10) {
            // The hash is a 32-bit number, so it cannot possibly be longer than 10 characters
            continue;
        }
        quint32 expected_hash = account_name.midRef(assumed_length, expected_hash_length).toUInt();

        static QStringList MSN_EMAIL_DOMAINS {"hotmail.com", "live.com", "outlook.com"};
        quint32 hash = update_messenger_hash(base_hash, '@');

        for (IMM(auto) domain : MSN_EMAIL_DOMAINS) {
            for (const QChar character : domain) {
                hash = update_messenger_hash(hash, character);
            }

            if (hash == expected_hash) {
                return FullAccountName(
                    IMProtocol::MSN,
                    account_name.mid(0, assumed_length).append('@').append(domain)
                );
            }
        }
    }

    invariant_violation("Could not decode MSN Messenger encoded account name: %s", qUtf8Printable(account_name));
}

bool is_likely_valid_optionally_encoded_msn_account_name(IMM(QString) account_name) {
    return is_valid_msn_account_name(account_name) || is_likely_valid_encoded_msn_account_name(account_name);
}

void assert_likely_valid_optionally_encoded_msn_account_name(IMM(QString) account_name) {
    invariant(
        is_likely_valid_optionally_encoded_msn_account_name(account_name),
        "'%s' doesn't look like a valid (optionally encoded) MSN account name",
        qUtf8Printable(account_name)
    );
}

FullAccountName parse_optionally_encoded_msn_account(IMM(QString) account_name) {
    return is_valid_msn_account_name(account_name)
           ? parse_msn_account(account_name) : parse_encoded_msn_account(account_name);
}

}}}
