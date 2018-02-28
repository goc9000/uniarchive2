/**
 * fixers/resolve_subjects/utils.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "fixers/resolve_subjects/utils.h"

#include "intermediate_format/subjects/ResolvedSubject.h"

#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace std;

void resolve_subject_in_place(unique_ptr<ApparentSubject>& mut_subject, IMM(QString) resolved_subject_id) {
    invariant(mut_subject->subType() != ApparentSubjectSubType::RESOLVED, "Subject is already resolved");

    ApparentSubject::Hints hints = mut_subject->hints;

    mut_subject = make_unique<ResolvedSubject>(resolved_subject_id, move(mut_subject), hints);
}

}}}
