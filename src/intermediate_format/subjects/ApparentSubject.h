/**
 * intermediate_format/subjects/ApparentSubject.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H

#include <QtDebug>

#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class ApparentSubject {
public:
    enum class Hint {
        NoHints    = 0,
        IsIdentity = (1 << 0),
        IsPeer     = (1 << 1),
    };
    Q_DECLARE_FLAGS(Hints, Hint);

    Hints hints;

    ApparentSubject(Hints hints = Hint::NoHints);

    virtual CEDE(ApparentSubject) clone() const = 0;

    virtual void writeToDebugStream(QDebug stream) const = 0;
};

QDebug operator<< (QDebug stream, ApparentSubject const * const subject);

Q_DECLARE_OPERATORS_FOR_FLAGS(ApparentSubject::Hints);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H
