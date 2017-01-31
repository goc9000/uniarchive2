/**
 * intermediate_format/subjects/ImplicitSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/ImplicitSubject.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ImplicitSubject::ImplicitSubject(ImplicitSubjectKind kind, Hints hints) : ApparentSubject(hints), kind(kind) {
    // Nothing else to initialize
}

ApparentSubjectSubType ImplicitSubject::subType() const {
    return ApparentSubjectSubType::IMPLICIT;
}

CEDE(ApparentSubject) ImplicitSubject::clone() const {
    return make_unique<ImplicitSubject>(kind, hints);
}

CEDE(ImplicitSubject) ImplicitSubject::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ApparentSubjectSubType::IMPLICIT);
    Hints hints = must_deserialize(mut_stream, Hints);

    return make_unique<ImplicitSubject>(must_deserialize(mut_stream, ImplicitSubjectKind), hints);
}

void ImplicitSubject::serializeToStreamSubImpl(QDataStream& mut_stream) const {
    mut_stream << kind;
}

void ImplicitSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:implicit:" << kind;
}

}}}
