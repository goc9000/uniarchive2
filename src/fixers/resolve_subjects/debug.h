/**
 * fixers/resolve_subjects/debug.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_DEBUG_H
#define UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_DEBUG_H

#include <map>

#include <QString>

#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace intermediate_format::subjects;
using namespace std;

struct UnresolvedSubjectsDB {
    void feed(IMM(unique_ptr<ApparentSubject>) subject);
    void dump() const;

protected:
    map<QString, int> subjects;
};

}}}

#endif //UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_DEBUG_H
