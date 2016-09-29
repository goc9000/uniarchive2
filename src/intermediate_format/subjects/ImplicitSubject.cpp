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
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ImplicitSubject::ImplicitSubject(ImplicitSubjectKind kind, Hints hints) : ApparentSubject(hints), kind(kind) {
}

ApparentSubjectSubType ImplicitSubject::subType() const {
    return ApparentSubjectSubType::IMPLICITSUBJECT;
}

CEDE(ApparentSubject) ImplicitSubject::clone() const {
    return make_unique<ImplicitSubject>(kind, hints);
}

void ImplicitSubject::serializeToStreamSubImpl(QDataStream& mut_stream) const {
    mut_stream << kind;
}

void ImplicitSubject::writeToDebugStreamImpl(QDebug stream) const {
    stream << "Sbj:implicit:" << kind;
}

}}}
