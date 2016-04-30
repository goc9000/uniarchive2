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
    virtual CEDE(ApparentSubject) clone() const = 0;

    virtual void writeToDebugStream(QDebug stream) const = 0;
};

QDebug operator<< (QDebug stream, ApparentSubject const * const subject);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECT_H
