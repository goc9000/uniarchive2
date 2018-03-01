/**
 * fixers/resolve_subjects/methods/by_accounts.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_METHODS_BY_ACCOUNTS_H
#define UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_METHODS_BY_ACCOUNTS_H

#include "intermediate_format/RawConversationCollection.h"
#include "intermediate_format/RawConversation.h"
#include "fixers/resolve_subjects/ResolveSubjectsConfig.h"

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace fixers { namespace resolve_subjects {

using namespace uniarchive2::intermediate_format;

void resolve_subjects_using_accounts(RawConversationCollection& mut_conversations, IMM(ResolveSubjectsConfig) config);
void resolve_subjects_using_accounts(RawConversation& mut_conversation, IMM(ResolveSubjectsConfig) config);

}}}

#endif //UNIARCHIVE2_FIXERS_RESOLVESUBJECTS_METHODS_BY_ACCOUNTS_H
