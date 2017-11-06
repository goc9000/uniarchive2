/**
 * fixers/resolve_subjects/ResolveSubjectsConfig.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "fixers/resolve_subjects/ResolveSubjectsConfig.h"

#include "protocols/parse_account_generic.h"

#include "utils/qt/shortcuts.h"
#include "utils/json/json_utils.h"
#include "utils/json/load_json.h"

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::json;

static void check_looks_like_id(IMM(QString) text);

ResolveSubjectsConfig ResolveSubjectsConfig::fromJSON(IMM(QJsonValue) json) {
    return ResolveSubjectsConfig::fromJSONImpl(expect_json_object(json));
}

ResolveSubjectsConfig ResolveSubjectsConfig::fromJSON(IMM(QJsonDocument) json) {
    return ResolveSubjectsConfig::fromJSONImpl(expect_json_object(json));
}

ResolveSubjectsConfig ResolveSubjectsConfig::loadFromFile(IMM(QString) filename) {
    return ResolveSubjectsConfig::fromJSON(load_json_file(filename));
}

ResolveSubjectsConfig ResolveSubjectsConfig::fromJSONImpl(IMM(QJsonObject) json) {
    ResolveSubjectsConfig config;

    for (IMM(QString) subject_id : json.keys()) {
        check_looks_like_id(subject_id);

        config.canonicalSubjects.emplace(
            piecewise_construct,
            forward_as_tuple(subject_id),
            forward_as_tuple(subject_id, json[subject_id])
        );
    }

    return config;
}


ResolveSubjectsConfigCanonicalSubject::ResolveSubjectsConfigCanonicalSubject(
    IMM(QString) subject_id,
    IMM(QJsonValue) json
) : canonicalID(subject_id) {
    check_looks_like_id(subject_id);
    QJsonObject as_obj = expect_json_object(json);

    for (IMM(QString) key : as_obj.keys()) {
        if (key == "accounts") {
            for (IMM(QJsonValue) item : expect_json_array(as_obj[key])) {
                accounts.push_back(parse_account_from_json(item));
            }
        } else {
            invariant_violation("Unrecognized key '%s' in Resolve Subjects Config for canonical subject", QP(key));
        }
    }
}


static void check_looks_like_id(IMM(QString) text) {
    QREGEX_MUST_MATCH(PATTERN, "^[a-z][a-z0-9_.-]*$", text, "'%s' does not look like an ID");
}

}}}
