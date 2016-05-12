/**
 * intermediate_format/subjects/ImplicitSubject.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECT_H

#include <memory>

#include <QtDebug>

#include "intermediate_format/subjects/ApparentSubject.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class ImplicitSubject : public ApparentSubject {
public:
    enum Kind {
        INVALID = 0,
        IDENTITY, // Whoever the identity (i.e. local user) is
        FILE_RECEIVER,
    };

    Kind kind;

    ImplicitSubject(Kind kind);

    virtual CEDE(ApparentSubject) clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECT_H
