/**
 * fixers/resolve_subjects/ResolveSubjectsConfig.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_RESOLVESUBJECTSCONFIG_H
#define UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_RESOLVESUBJECTSCONFIG_H

#include "protocols/FullAccountName.h"

#include "utils/language/shortcuts.h"

#include <map>
#include <vector>

#include <QString>
#include <QJsonDocument>
#include <QJsonValue>

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace uniarchive2::protocols;
using namespace std;

struct ResolveSubjectsConfigCanonicalSubject;

struct ResolveSubjectsConfig {
    map<QString, ResolveSubjectsConfigCanonicalSubject> canonicalSubjects;

    static ResolveSubjectsConfig fromJSON(IMM(QJsonValue) json);
    static ResolveSubjectsConfig fromJSON(IMM(QJsonDocument) json);
    static ResolveSubjectsConfig loadFromFile(IMM(QString) filename);

protected:
    static ResolveSubjectsConfig fromJSONImpl(IMM(QJsonObject) json);
};

struct ResolveSubjectsConfigCanonicalSubject {
    QString canonicalID;
    vector<FullAccountName> accounts;

    ResolveSubjectsConfigCanonicalSubject(IMM(QString) subject_id, IMM(QJsonValue) json);
};

}}}

#endif //UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_RESOLVESUBJECTSCONFIG_H
