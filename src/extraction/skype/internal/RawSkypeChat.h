/**
 * extraction/skype/internal/RawSkypeConvo.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECHAT_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECHAT_H

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <set>

#include <QString>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

using namespace std::experimental;

struct RawSkypeChat {
    enum class Type {
        ONE_ON_ONE = 2,
        GROUP_CHAT = 4
    };

    uint64_t id;
    QString stringID;
    Type type;
    QString friendlyName;
    uint64_t timestamp;
    optional<QString> lastTopic;
    optional<QString> lastTopicXML;
    uint64_t convDBID;

    set<QString> participants;

    RawSkypeChat(
        uint64_t id,
        IMM(QString) string_id,
        Type type,
        IMM(QString) friendly_name,
        uint64_t timestamp,
        IMM(optional<QString>) last_topic,
        IMM(optional<QString>) last_topic_xml,
        uint64_t conv_dbid
    ) : id(id), stringID(string_id), type(type), friendlyName(friendly_name), timestamp(timestamp),
        lastTopic(last_topic), lastTopicXML(last_topic_xml), convDBID(conv_dbid) {
        // Nothing else to initialize
    }
};

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECHAT_H
