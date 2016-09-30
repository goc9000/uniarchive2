/**
 * intermediate_format/events/file_transfer/RawStartFileTransferEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): fab9ceaa1acb9d32649b467dfab37ae0efe2ddb8650aee69710c7a1729dc1d59
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTFILETRANSFEREVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTFILETRANSFEREVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/RawTransferredFile.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawStartFileTransferEvent : RawEvent {
public:
    RawTransferredFile file;
    unique_ptr<ApparentSubject> sender;

    RawStartFileTransferEvent(IMM(ApparentTime) timestamp, uint index, IMM(RawTransferredFile) file);

    virtual RawEventSubType subType() const;

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTFILETRANSFEREVENT_H
