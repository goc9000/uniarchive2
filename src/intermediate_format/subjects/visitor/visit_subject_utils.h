/**
 * intermediate_format/subjects/visitor/visit_subject_utils.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_VISITSUBJECTUTILS_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_VISITSUBJECTUTILS_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/visitor/visit_subjects_callback.h"
#include "intermediate_format/subjects/visitor/IApparentSubjectVisitable.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace std::experimental;

bool visit_subjects(IApparentSubjectVisitable& visitable, IMM(visit_subjects_callback_t) callback);
bool visit_subjects(unique_ptr<ApparentSubject>& subject, IMM(visit_subjects_callback_t) callback);

template<typename T>
bool visit_subjects(optional<T>& item, IMM(visit_subjects_callback_t) callback) {
    if (!item) {
        return true;
    }

    return visit_subjects(*item, callback);
}

template<typename T>
bool visit_subjects(unique_ptr<T>& item, IMM(visit_subjects_callback_t) callback) {
    if (!item) {
        return true;
    }

    return visit_subjects(*item, callback);
}

template<typename T>
bool visit_subjects(vector<T>& items, IMM(visit_subjects_callback_t) callback) {
    for (size_t i = 0; i < items.size(); i++) {
        if (!visit_subjects(items.at(i), callback)) {
            return false;
        }
    }

    return true;
}

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_VISITSUBJECTUTILS_H
