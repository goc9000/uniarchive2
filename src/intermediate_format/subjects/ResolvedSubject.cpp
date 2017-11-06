/**
 * intermediate_format/subjects/ResolvedSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/ResolvedSubject.h"
#include "utils/serialization/serialization_helpers.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/qt/shortcuts.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ResolvedSubject::ResolvedSubject(IMM(QString) subject_id, TAKE(ApparentSubject) original_subject, Hints hints)
  : ApparentSubject(hints), canonicalSubjectID(subject_id), originalSubject(move(original_subject)) {
    invariant(!!originalSubject, "Must provide an original subject");
}

ApparentSubjectSubType ResolvedSubject::subType() const {
    return ApparentSubjectSubType::RESOLVED;
}

CEDE(ApparentSubject) ResolvedSubject::clone() const {
    return make_unique<ResolvedSubject>(canonicalSubjectID, originalSubject->clone(), hints);
}

CEDE(ResolvedSubject) ResolvedSubject::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ApparentSubjectSubType::RESOLVED);
    Hints hints = must_deserialize(mut_stream, Hints);

    return make_unique<ResolvedSubject>(
        must_deserialize(mut_stream, QString),
        must_deserialize(mut_stream, unique_ptr<ApparentSubject>),
        hints
    );
}

void ResolvedSubject::serializeToStreamSubImpl(QDataStream &mut_stream) const {
    mut_stream << canonicalSubjectID << originalSubject;
}

void ResolvedSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:resolved:" << QP(canonicalSubjectID) << "<" << originalSubject << ">";
}

}}}
