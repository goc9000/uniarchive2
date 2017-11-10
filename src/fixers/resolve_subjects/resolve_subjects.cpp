/**
 * fixers/resolve_subjects/resolve_subjects.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "fixers/resolve_subjects/resolve_subjects.h"

#include "intermediate_format/subjects/visitor/IApparentSubjectVisitor.h"
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

class ResolveSubjectsProcess : IApparentSubjectVisitor {
public:
    ResolveSubjectsProcess(IMM(ResolveSubjectsConfig) config) : config(config) {
        init();
    }

    void run(RawConversationCollection& mut_conversations) {
        mut_conversations.visitSubjects(*this);
        finish();
    }

    void run(RawConversation& mut_conversation) {
        mut_conversation.visitSubjects(*this);
        finish();
    }

protected:
    IMM(ResolveSubjectsConfig) config;

    map<IMProtocol, map<QString, QString>> accountsIndex;
    map<QString, int> debugUnresolvedSubjects;

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

        ApparentSubjectSubType subtype = mut_subject->subType();

        if ((subtype == ApparentSubjectSubType::IMPLICIT) || (subtype == ApparentSubjectSubType::RESOLVED)) {
            return true; // Leave implicit and already resolved subjects alone
        }

        tryExactAccountMatch(mut_subject) ||
            unresolvedSubject(mut_subject);

        return true;
    }

    bool tryExactAccountMatch(unique_ptr<ApparentSubject>& mut_subject) {
        ApparentSubjectSubType subtype = mut_subject->subType();

        if (subtype == ApparentSubjectSubType::ACCOUNT) {
            return tryExactAccountMatchImpl(mut_subject->as<AccountSubject>()->account, mut_subject);
        } else if (subtype == ApparentSubjectSubType::FULLY_SPECIFIED) {
            return tryExactAccountMatchImpl(mut_subject->as<FullySpecifiedSubject>()->accountName, mut_subject);
        } else {
            return false;
        }
    }

    bool tryExactAccountMatchImpl(IMM(FullAccountName) account, unique_ptr<ApparentSubject>& mut_subject) {
        if (accountsIndex.count(account.protocol) && accountsIndex.at(account.protocol).count(account.accountName)) {
            return resolve(mut_subject, accountsIndex.at(account.protocol).at(account.accountName));
        }

        return trySkypeOnMSNMatch(account, mut_subject);
    }

    bool trySkypeOnMSNMatch(IMM(FullAccountName) account, unique_ptr<ApparentSubject>& mut_subject) {
        const QString SKYPE_ON_MSN_DOMAIN = "@fakeskypedomain.fakedomain";

        if (!((account.protocol == IMProtocol::MSN) && account.accountName.endsWith(SKYPE_ON_MSN_DOMAIN))) {
            return false;
        }
        if (!accountsIndex.count(IMProtocol::SKYPE)) {
            return false;  // Unlikely, but...
        }
        IMM(auto) skype_accounts = accountsIndex.at(IMProtocol::SKYPE);

        QString try_account_name =
            account.accountName.left(account.accountName.length() - SKYPE_ON_MSN_DOMAIN.length());

        if (skype_accounts.count(try_account_name)) {
            return resolve(mut_subject, skype_accounts.at(try_account_name));
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
            return resolve(mut_subject, found_value);
        }

        return false;
    }

    bool resolve(unique_ptr<ApparentSubject>& mut_subject, IMM(QString) resolved_subject_id) {
        ApparentSubject::Hints hints = mut_subject->hints;

        mut_subject = make_unique<ResolvedSubject>(resolved_subject_id, move(mut_subject), hints);

        return true;
    }

    bool unresolvedSubject(IMM(unique_ptr<ApparentSubject>) subject) {
        QString repr;
        QDebug(&repr) << subject;

        repr = repr.replace(" [peer]", "").replace(" [ident]", "").trimmed();

        debugUnresolvedSubjects[repr]++;

        return true;
    }

    void finish() {
        // TODO: remove temporary debug printout
        dumpUnresolvedSubjects();
    }

    void dumpUnresolvedSubjects() {
        if (debugUnresolvedSubjects.empty()) {
            return;
        }

        vector<pair<QString, int>> results;
        int total_occurrences = 0;

        for (IMM(auto) kv : debugUnresolvedSubjects) {
            results.push_back(kv);
            total_occurrences += kv.second;
        }

        sort(results.begin(), results.end(), [](auto &a, auto &b) { return a.second > b.second; });

        QString summary =
            QString("%1 unresolved subjects, %2 total occcurences").arg(results.size()).arg(total_occurrences);
        qDebug() << QP(summary);
        qDebug() << "=================================================================";

        int lines_printed = 0;
        int MAX_LINES = 200;

        for (IMM(auto) kv : results) {
            qDebug() << QP(QString("%1 x %2").arg(kv.second, 5).arg(kv.first));

            lines_printed++;
            if (lines_printed >= MAX_LINES) {
                qDebug() << QP(QString("...(%1 more lines)...").arg(results.size() - lines_printed));
                break;
            }
        }
    }
};

void resolve_subjects(RawConversationCollection& mut_conversations, IMM(ResolveSubjectsConfig) config) {
    ResolveSubjectsProcess(config).run(mut_conversations);
}

void resolve_subjects(RawConversation& mut_conversation, IMM(ResolveSubjectsConfig) config) {
    ResolveSubjectsProcess(config).run(mut_conversation);
}

}}}
