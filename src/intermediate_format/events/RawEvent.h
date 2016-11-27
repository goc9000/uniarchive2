/**
 * intermediate_format/events/RawEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8cf43c53fdb15dbcaace373193cf0ec9a9803e8d811b362e3434ca2f2e2f98c6
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H

#include "intermediate_format/events/RawEventSubType.h"
#include "intermediate_format/ApparentTime.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"
#include "utils/polymorphics/StandardPolymorphic.h"

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std::experimental;
using namespace uniarchive2::utils::polymorphics;

struct RawEvent : StandardPolymorphic<RawEventSubType> {
public:
    ApparentTime timestamp;
    uint32_t indexInConversation;

    optional<QByteArray> skypeGUID;

    RawEvent(IMM(ApparentTime) timestamp, uint32_t index);

    virtual QString eventName() const;

    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;
    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H
