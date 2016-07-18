/**
 * intermediate_format/events/file_transfer/RawCancelFileTransferEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCANCELFILETRANSFEREVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCANCELFILETRANSFEREVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/RawTransferredFile.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;

struct RawCancelFileTransferEvent : RawEvent {
public:
    optional<RawTransferredFile> file;
    unique_ptr<ApparentSubject> actor;

    RawCancelFileTransferEvent(IMM(ApparentTime) timestamp, unsigned int index);
    RawCancelFileTransferEvent(IMM(ApparentTime) timestamp, unsigned int index, IMM(RawTransferredFile) file);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCANCELFILETRANSFEREVENT_H
