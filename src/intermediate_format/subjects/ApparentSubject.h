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

#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace std;

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

QDebug operator<< (QDebug stream, CPTR(ApparentSubject) subject);

Q_DECLARE_OPERATORS_FOR_FLAGS(ApparentSubject::Hints);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H
