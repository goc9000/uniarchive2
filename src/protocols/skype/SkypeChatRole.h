/**
 * protocols/skype/SkypeChatRole.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_SKYPE_SKYPECHATROLE_H
#define UNIARCHIVE2_PROTOCOLS_SKYPE_SKYPECHATROLE_H

#include <QDebug>

namespace uniarchive2 { namespace protocols { namespace skype {

// As described on: https://support.skype.com/en/faq/FA10042/what-are-chat-commands-and-roles

enum class SkypeChatRole {
    INVALID = 0,
    CREATOR, // can promote others to master
    MASTER, // can promote others to helper (at most)
    HELPER, // slightly privileged user (e.g. exempt from USERS_ARE_LISTENERS option)
    REGULAR_USER,
    LISTENER, // cannot post
};

QString name_for_skype_chat_role(SkypeChatRole role);

QDebug operator<< (QDebug stream, SkypeChatRole role);

}}}

#endif //UNIARCHIVE2_PROTOCOLS_SKYPE_SKYPECHATROLE_H