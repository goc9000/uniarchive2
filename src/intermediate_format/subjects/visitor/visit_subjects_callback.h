/**
 * intermediate_format/subjects/visitor/visit_subjects_callback.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_VISIT_SUBJECTS_CALLBACK_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_VISIT_SUBJECTS_CALLBACK_H

#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace std;

typedef function<bool(IMM(unique_ptr<ApparentSubject>))> visit_subjects_callback_t;
typedef function<bool(unique_ptr<ApparentSubject>&)> alter_subjects_callback_t;

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_VISIT_SUBJECTS_CALLBACK_H
