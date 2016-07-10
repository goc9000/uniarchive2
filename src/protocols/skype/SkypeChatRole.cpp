/**
 * protocols/skype/SkypeChatRole.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/skype/SkypeChatRole.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols { namespace skype {

QString name_for_skype_chat_role(SkypeChatRole role) {
    switch (role) {
        case SkypeChatRole::INVALID:
            return "(invalid)";
        case SkypeChatRole::CREATOR:
            return "creator";
        case SkypeChatRole::MASTER:
            return "master";
        case SkypeChatRole::HELPER:
            return "helper";
        case SkypeChatRole::REGULAR_USER:
            return "regular_user";
        case SkypeChatRole::LISTENER:
            return "listener";
    }
}

QDebug operator<< (QDebug stream, SkypeChatRole role) {
    stream << QP(name_for_skype_chat_role(role));

    return stream;
}

}}}
