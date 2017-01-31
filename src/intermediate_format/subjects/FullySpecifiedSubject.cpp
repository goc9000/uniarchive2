/**
 * intermediate_format/subjects/FullySpecifiedSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

FullySpecifiedSubject::FullySpecifiedSubject(
    IMM(FullAccountName) account_name,
    IMM(QString) screen_name,
    Hints hints
) : ApparentSubject(hints), accountName(account_name), screenName(screen_name) {
    // Nothing else to initialize
}

ApparentSubjectSubType FullySpecifiedSubject::subType() const {
    return ApparentSubjectSubType::FULLY_SPECIFIED;
}

CEDE(ApparentSubject) FullySpecifiedSubject::clone() const {
    return make_unique<FullySpecifiedSubject>(accountName, screenName, hints);
}

CEDE(FullySpecifiedSubject) FullySpecifiedSubject::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ApparentSubjectSubType::FULLY_SPECIFIED);
    Hints hints = must_deserialize(mut_stream, Hints);

    return make_unique<FullySpecifiedSubject>(
        must_deserialize(mut_stream, FullAccountName),
        must_deserialize(mut_stream, QString),
        hints
    );
}

void FullySpecifiedSubject::serializeToStreamSubImpl(QDataStream &mut_stream) const {
    mut_stream << accountName << screenName;
}

void FullySpecifiedSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:full:" << QP(screenName) << " [" << accountName << "]";
}

}}}
