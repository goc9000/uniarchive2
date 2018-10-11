/**
 * fixers/resolve_subjects/debug.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "fixers/resolve_subjects/debug.h"

#include "intermediate_format/subjects/visitor/visit_subject_utils.h"

#include "utils/qt/shortcuts.h"
#include "utils/qt/debug_extras.h"

#include <map>
#include <vector>
#include <algorithm>

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace std;

static QString summarize_subject(IMM(unique_ptr<ApparentSubject>) subject) {
    QString repr;
    QDebug(&repr) << subject;

    return repr.replace(" [peer]", "").replace(" [ident]", "").trimmed();
}

static void dump_unresolved_subjects(const map<QString, int>& subjects) {
    if (subjects.empty()) {
        return;
    }

    vector<pair<QString, int>> results;
    int total_occurrences = 0;

    for (IMM(auto) kv : subjects) {
        results.push_back(kv);
        total_occurrences += kv.second;
    }

    sort(results.begin(), results.end(), [](auto &a, auto &b) { return a.second > b.second; });

    QString summary =
        QString("%1 unresolved subjects, %2 total occcurences").arg(results.size()).arg(total_occurrences);
    qDebug() << QP(summary);
    qDebug() << "=================================================================";

    int lines_printed = 0;
    int MAX_LINES = 2000;

    for (IMM(auto) kv : results) {
        qDebug() << QP(QString("%1 x %2").arg(kv.second, 5).arg(kv.first));

        lines_printed++;
        if (lines_printed >= MAX_LINES) {
            qDebug() << QP(QString("...(%1 more lines)...").arg(results.size() - lines_printed));
            break;
        }
    }
}

void analyze_unresolved_subjects(IMM(RawConversationCollection) conversations) {
    map<QString, int> subjects;

    for (IMM(auto) convo : conversations) {
        QString base;
        QDebug(&base) << convo.protocol;

        convo.visitSubjects([&](IMM(unique_ptr<ApparentSubject>) subject) {
            if (subject->subType() != ApparentSubjectSubType::RESOLVED) {
                subjects[base + summarize_subject(subject)]++;
            }
            return true;
        });
    }

    dump_unresolved_subjects(subjects);
}

}}}
