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

#include "intermediate_format/RawConversationCollection.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace intermediate_format;

void analyze_unresolved_subjects(IMM(RawConversationCollection) conversations);

}}}

#endif //UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_DEBUG_H
