/**
 * intermediate_format/events/RawEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 94d21897766fd95461112a07736b147e06c2f8a766b24281feb8cd291b1488f7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H

#include "intermediate_format/ApparentTime.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/polymorphic_helpers.h"
#include "utils/language/shortcuts.h"

#include <QByteArray>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std::experimental;

struct RawEvent {
public:
    ApparentTime timestamp;
    uint indexInConversation;

    optional<QByteArray> skypeGUID;

    RawEvent(IMM(ApparentTime) timestamp, uint index);

    POLYMORPHIC_HELPERS

    virtual QString eventName() const = 0;
    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

QDebug operator<< (QDebug stream, CPTR(RawEvent) event);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H
