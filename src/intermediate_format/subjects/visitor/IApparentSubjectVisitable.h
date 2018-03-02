/**
 * intermediate_format/subjects/visitor/IApparentSubjectVisitable.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IAPPARENTSUBJECTVISITABLE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IAPPARENTSUBJECTVISITABLE_H

#include "intermediate_format/subjects/visitor/visit_subjects_callback.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

class IApparentSubjectConstVisitable {
public:
    virtual bool visitSubjects(IMM(visit_subjects_callback_t) callback) const = 0;
};

class IApparentSubjectVisitable : public IApparentSubjectConstVisitable {
public:
    virtual bool alterSubjects(IMM(alter_subjects_callback_t) callback) = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IAPPARENTSUBJECTVISITABLE_H
