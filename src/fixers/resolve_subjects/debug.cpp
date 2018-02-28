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

#include "utils/qt/shortcuts.h"
#include "utils/qt/debug_extras.h"

#include <map>
#include <vector>
#include <algorithm>

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace std;

void UnresolvedSubjectsDB::feed(IMM(unique_ptr<ApparentSubject>) subject) {
    QString repr;
    QDebug(&repr) << subject;

    repr = repr.replace(" [peer]", "").replace(" [ident]", "").trimmed();

    subjects[repr]++;
}

void UnresolvedSubjectsDB::dump() const {
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

}}}
