/**
 * intermediate_format/subjects/visitor/visit_subject_utils.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/visitor/visit_subject_utils.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

bool visit_subjects(IApparentSubjectVisitable& visitable, IApparentSubjectVisitor& visitor) {
    return visitable.visitSubjects(visitor);
}

bool visit_subjects(unique_ptr<ApparentSubject>& subject, IApparentSubjectVisitor& visitor) {
    if (!subject) {
        return true;
    }

    return visitor.visit(subject);
}

}}}
