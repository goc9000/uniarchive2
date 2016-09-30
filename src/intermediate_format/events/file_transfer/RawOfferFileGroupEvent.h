/**
 * intermediate_format/events/file_transfer/RawOfferFileGroupEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3b8786023ea657e4f61724f1345282b4346db769682507abab38e6c8bce08f71
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERFILEGROUPEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERFILEGROUPEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawOfferFileGroupEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> recipient;
    uint numFiles;

    RawOfferFileGroupEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) sender, uint num_files);

    virtual RawEventSubType subType() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERFILEGROUPEVENT_H
