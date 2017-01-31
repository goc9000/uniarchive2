/**
 * extraction/skype/internal/RawSkypeCall.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECALL_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECALL_H

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <map>

#include <QString>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

using namespace std::experimental;

struct RawSkypeCall {
    uint64_t id;
    QString hostIdentity;
    bool isIncoming;
    uint64_t beginTimestamp;
    optional<uint> duration;
    QString topic;
    QString internalName;
    uint64_t convDBID;

    map<QString, QString> participants;

    RawSkypeCall(
        uint64_t id,
        IMM(QString) host_identity,
        bool is_incoming,
        uint64_t begin_timestamp,
        IMM(optional<uint>) duration,
        QString topic,
        QString internal_name,
        uint64_t conv_dbid
    ) : id(id), hostIdentity(host_identity), isIncoming(is_incoming), beginTimestamp(begin_timestamp),
        duration(duration), topic(topic), internalName(internal_name), convDBID(conv_dbid) {
        // Nothing else to initialize
    }
};

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPECALL_H
