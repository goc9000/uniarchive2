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

    map<QString, int> debugUnresolvedSubjects;

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
        for (IMM(auto) kv : config.canonicalSubjects) {
            for (IMM(auto) candidate_account : kv.second.accounts) {
                if (account == candidate_account) {
                    return resolve(mut_subject, kv.first);
                }
            }
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
