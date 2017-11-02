/**
 * intermediate_format/subjects/visitor/ApparentSubjectVisitable.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECTVISITABLE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECTVISITABLE_H

#include "intermediate_format/subjects/visitor/ApparentSubjectVisitor.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class ApparentSubjectVisitable {
public:
    virtual void visitSubjects(ApparentSubjectVisitor& visitor) = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_APPARENTSUBJECTVISITABLE_H
