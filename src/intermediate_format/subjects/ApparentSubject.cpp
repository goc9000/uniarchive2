/**
 * intermediate_format/subjects/ApparentSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/ScreenNameSubject.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ApparentSubject::ApparentSubject(Hints hints) : hints(hints) {
    // Nothing else to initialize
}

CEDE(ApparentSubject) ApparentSubject::deserializeFromStream(QDataStream& mut_stream) {
    ApparentSubjectSubType subtype = must_deserialize(mut_stream, ApparentSubjectSubType);

    switch (subtype) {
        case ApparentSubjectSubType::IMPLICIT:
            return ImplicitSubject::deserializeFromStream(mut_stream, true);
        case ApparentSubjectSubType::SCREEN_NAME:
            return ScreenNameSubject::deserializeFromStream(mut_stream, true);
        case ApparentSubjectSubType::ACCOUNT:
            return AccountSubject::deserializeFromStream(mut_stream, true);
        case ApparentSubjectSubType::FULLY_SPECIFIED:
            return FullySpecifiedSubject::deserializeFromStream(mut_stream, true);
    }

    invariant_violation("Invalid deserialized ApparentSubject subtype (code: %d)", (int)subtype);
}

void ApparentSubject::serializeToStreamImpl(QDataStream &mut_stream) const {
    mut_stream << hints;
    serializeToStreamSubImpl(mut_stream);
}

void ApparentSubject::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver save(stream);
    stream.nospace();
    writeToDebugStreamImpl(stream);

    if (hints) {
        QStringList hints_repr;

        if (hints.testFlag(ApparentSubject::Hint::IsIdentity)) {
            hints_repr << "ident";
        }
        if (hints.testFlag(ApparentSubject::Hint::IsPeer)) {
            hints_repr << "peer";
        }

        stream << " [" << QP(hints_repr.join(',')) << "]";
    }
}

}}}
