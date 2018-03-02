/**
 * fixers/resolve_subjects/methods/by_accounts.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "fixers/resolve_subjects/methods/by_accounts.h"
#include "fixers/resolve_subjects/utils.h"
#include "fixers/resolve_subjects/debug.h"

#include "intermediate_format/subjects/visitor/visit_subjects_callback.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/ResolvedSubject.h"

#include "utils/qt/shortcuts.h"
#include "utils/qt/debug_extras.h"

#include <map>
#include <vector>
#include <algorithm>

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace intermediate_format::subjects;
using namespace std;

/**
 * This is the simplest and most exact subject resolution method. It takes any subject that refers to an account, i.e.
 * account-based or "fully specified" subjects, and looks up the account in a ResolveSubjectsConfig database that lists
 * canonical subject IDs and their known acccounts.
 *
 * This step has no prerequisites and should be run before any other subject resolution methods.
 */
class ResolveSubjectsUsingAccountsProcess {
public:
    ResolveSubjectsUsingAccountsProcess(IMM(ResolveSubjectsConfig) config) : config(config) {
        init();
    }

    void run(RawConversationCollection& mut_conversations) {
        mut_conversations.visitSubjects([this](unique_ptr<ApparentSubject>& subject){ return visit(subject); });
        finish();
    }

    void run(RawConversation& mut_conversation) {
        mut_conversation.visitSubjects([this](unique_ptr<ApparentSubject>& subject){ return visit(subject); });
        finish();
    }

protected:
    IMM(ResolveSubjectsConfig) config;

    map<IMProtocol, map<QString, QString>> accountsIndex;

    void init() {
        indexAccounts();
    }

    void indexAccounts() {
        for (IMM(auto) kv : config.canonicalSubjects) {
            for (IMM(auto) account : kv.second.accounts) {
                invariant(
                    !accountsIndex[account.protocol].count(account.accountName),
                    "Duplicate account: %s@%s", QP(account.accountName), QP(symbol_for_im_protocol(account.protocol))
                );

                accountsIndex[account.protocol][account.accountName] = kv.first;
            }
        }
    }

    bool visit(unique_ptr<ApparentSubject>& mut_subject) {
        invariant(!!mut_subject, "Did not expect unset subject!");

        switch (mut_subject->subType()) {
            case ApparentSubjectSubType::ACCOUNT:
                tryExactAccountMatchImpl(mut_subject->as<AccountSubject>()->account, mut_subject);
                break;
            case ApparentSubjectSubType::FULLY_SPECIFIED:
                tryExactAccountMatchImpl(mut_subject->as<FullySpecifiedSubject>()->accountName, mut_subject);
                break;
            default:
                break;
        }

        return true;
    }

    void tryExactAccountMatchImpl(IMM(FullAccountName) account, unique_ptr<ApparentSubject>& mut_subject) {
        if (accountsIndex.count(account.protocol) && accountsIndex.at(account.protocol).count(account.accountName)) {
            resolve_subject_in_place(mut_subject, accountsIndex.at(account.protocol).at(account.accountName));
        } else {
            trySkypeOnMSNMatch(account, mut_subject);
        }
    }

    void trySkypeOnMSNMatch(IMM(FullAccountName) account, unique_ptr<ApparentSubject>& mut_subject) {
        const QString SKYPE_ON_MSN_DOMAIN = "@fakeskypedomain.fakedomain";

        if (!((account.protocol == IMProtocol::MSN) && account.accountName.endsWith(SKYPE_ON_MSN_DOMAIN))) {
            return;
        }
        if (!accountsIndex.count(IMProtocol::SKYPE)) {
            return;  // Unlikely, but...
        }
        IMM(auto) skype_accounts = accountsIndex.at(IMProtocol::SKYPE);

        QString try_account_name =
            account.accountName.left(account.accountName.length() - SKYPE_ON_MSN_DOMAIN.length());

        if (skype_accounts.count(try_account_name)) {
            resolve_subject_in_place(mut_subject, skype_accounts.at(try_account_name));
            return;
        }

        // Seems periods in the Skype name are replaced with underscores in the MSN equivalent
        QString found_value;
        int found_times = 0;

        for (IMM(auto) kv : skype_accounts) {
            QString msn_name = QString(kv.first).replace('.', '_');

            if (msn_name == try_account_name) {
                found_value = kv.second;
                found_times++;
            }
        }

        invariant(found_times <= 1, "Ambiguous match for MSN account: %s", QP(account.accountName));

        if (found_times) {
            resolve_subject_in_place(mut_subject, found_value);
        }
    }

    void finish() {
        // Nothing to do
    }
};

void resolve_subjects_using_accounts(RawConversationCollection& mut_conversations, IMM(ResolveSubjectsConfig) config) {
    ResolveSubjectsUsingAccountsProcess(config).run(mut_conversations);
}

void resolve_subjects_using_accounts(RawConversation& mut_conversation, IMM(ResolveSubjectsConfig) config) {
    ResolveSubjectsUsingAccountsProcess(config).run(mut_conversation);
}

}}}
