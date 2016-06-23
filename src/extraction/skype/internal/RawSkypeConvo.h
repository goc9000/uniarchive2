/**
 * extraction/skype/internal/RawSkypeConvo.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECONVO_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECONVO_H

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <set>

#include <QString>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

using namespace std::experimental;

struct RawSkypeConvo {
    enum class Type {
        ONE_ON_ONE = 1,
        GROUP_CHAT = 2,
        CALL = 4
    };

    uint64_t id;
    Type type;
    QString identity;
    QString displayName;
    optional<QString> givenDisplayName;
    optional<uint64_t> spawnedFrom;
    optional<QString> creator;
    uint64_t timeCreated;
    optional<QString> lastTopic;

    set<QString> participants;

    RawSkypeConvo(
        uint64_t id,
        Type type,
        IMM(QString) identity,
        IMM(QString) display_name,
        IMM(optional<QString>) given_display_name,
        IMM(optional<uint64_t>) spawned_from,
        IMM(optional<QString>) creator,
        uint64_t time_created,
        IMM(optional<QString>) last_topic
    ) : id(id), type(type), identity(identity), displayName(display_name), givenDisplayName(given_display_name),
        spawnedFrom(spawned_from), creator(creator), timeCreated(time_created), lastTopic(last_topic) {}
};

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECONVO_H
