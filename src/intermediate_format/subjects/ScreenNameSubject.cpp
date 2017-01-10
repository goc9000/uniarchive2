/**
 * intermediate_format/subjects/ScreenNameSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/ScreenNameSubject.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ScreenNameSubject::ScreenNameSubject(IMM(QString) screen_name, Hints hints)
  : ApparentSubject(hints), screenName(screen_name) {
}

ApparentSubjectSubType ScreenNameSubject::subType() const {
    return ApparentSubjectSubType::SCREEN_NAME;
}

CEDE(ApparentSubject) ScreenNameSubject::clone() const {
    return make_unique<ScreenNameSubject>(screenName, hints);
}

CEDE(ScreenNameSubject) ScreenNameSubject::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ApparentSubjectSubType::SCREEN_NAME);
    Hints hints = must_deserialize(mut_stream, Hints);

    return make_unique<ScreenNameSubject>(must_deserialize(mut_stream, QString), hints);
}

void ScreenNameSubject::serializeToStreamSubImpl(QDataStream &mut_stream) const {
    mut_stream << screenName;
}

void ScreenNameSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:scrname:" << QP(screenName);
}

}}}
