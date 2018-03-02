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

bool visit_subjects(IMM(IApparentSubjectConstVisitable) visitable, IMM(visit_subjects_callback_t) callback) {
    return visitable.visitSubjects(callback);
}

bool alter_subjects(IApparentSubjectVisitable& visitable, IMM(alter_subjects_callback_t) callback) {
    return visitable.alterSubjects(callback);
}

bool visit_subjects(IMM(unique_ptr<ApparentSubject>) subject, IMM(visit_subjects_callback_t) callback) {
    if (!subject) {
        return true;
    }

    return callback(subject);
}

bool alter_subjects(unique_ptr<ApparentSubject>& subject, IMM(alter_subjects_callback_t) callback) {
    if (!subject) {
        return true;
    }

    return callback(subject);
}

}}}
