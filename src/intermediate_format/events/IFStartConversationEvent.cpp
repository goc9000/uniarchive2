/**
 * intermediate_format/events/IFStartConversationEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFStartConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFStartConversationEvent::IFStartConversationEvent(ApparentTime timestamp, unsigned int index)
    : IntermediateFormatEvent(timestamp, index) {
}

IFStartConversationEvent::IFStartConversationEvent(
    ApparentTime timestamp,
    unsigned int index,
    shared_ptr<ApparentSubject> initiator
): IntermediateFormatEvent(timestamp, index), initiator(initiator) {
}

QString IFStartConversationEvent::eventName() const {
    return "StartConversation";
}

void IFStartConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (initiator) {
        stream << " initiator=" << initiator.get();
    }
}

}}}