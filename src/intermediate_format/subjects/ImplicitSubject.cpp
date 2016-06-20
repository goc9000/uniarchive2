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

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ImplicitSubject::ImplicitSubject(Kind kind, Hints hints) : ApparentSubject(hints), kind(kind) {
}

CEDE(ApparentSubject) ImplicitSubject::clone() const {
    return make_unique<ImplicitSubject>(kind, hints);
}

QString name_for_implicit_subject_kind(ImplicitSubject::Kind kind) {
    switch (kind) {
        case ImplicitSubject::Kind::INVALID:
            return "(invalid)";
        case ImplicitSubject::Kind::IDENTITY:
            return "identity";
        case ImplicitSubject::Kind::FILE_RECEIVER:
            return "file_receiver";
    }

    invariant_violation("Unhandled enum value");
}

void ImplicitSubject::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "Sbj:implicit:" << QP(name_for_implicit_subject_kind(kind));
}

}}}
