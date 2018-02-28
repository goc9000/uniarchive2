/**
 * fixers/resolve_subjects/utils.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_UTILS_H
#define UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_UTILS_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace intermediate_format::subjects;
using namespace std;

void resolve_subject_in_place(unique_ptr<ApparentSubject>& mut_subject, IMM(QString) resolved_subject_id);

}}}

#endif //UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_UTILS_H
