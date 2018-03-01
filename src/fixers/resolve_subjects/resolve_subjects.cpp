/**
 * fixers/resolve_subjects/resolve_subjects.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "fixers/resolve_subjects/resolve_subjects.h"
#include "fixers/resolve_subjects/methods/by_accounts.h"

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace intermediate_format::subjects;

void resolve_subjects(RawConversationCollection& mut_conversations, IMM(ResolveSubjectsConfig) config) {
    resolve_subjects_using_accounts(mut_conversations, config);
}

void resolve_subjects(RawConversation& mut_conversation, IMM(ResolveSubjectsConfig) config) {
    resolve_subjects_using_accounts(mut_conversation, config);
}

}}}
