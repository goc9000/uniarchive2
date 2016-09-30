/**
 * intermediate_format/events/file_transfer/RawCancelFileTransferEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 726e0912173e316cf39e810946ff87841ec651014c73f8cce46e74cc9c212b49
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

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawCancelFileTransferEvent : RawEvent {
public:
    optional<RawTransferredFile> file;
    unique_ptr<ApparentSubject> actor;

    RawCancelFileTransferEvent(IMM(ApparentTime) timestamp, uint index);
    RawCancelFileTransferEvent(IMM(ApparentTime) timestamp, uint index, IMM(RawTransferredFile) file);

    virtual RawEventSubType subType() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCANCELFILETRANSFEREVENT_H
