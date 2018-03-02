/**
 * intermediate_format/events/file_transfer/RawTransferFilesEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3188a7fc4505f7c3653204a3a0e84dd527802176a8e02f017766bbac064bd496
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWTRANSFERFILESEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWTRANSFERFILESEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/RawTransferredFile.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawTransferFilesEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> recipient;
    vector<RawTransferredFile> files;

    RawTransferFilesEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) sender,
        IMM(vector<RawTransferredFile>) files
    );
    RawTransferFilesEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) sender,
        IMM(RawTransferredFile) file
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawTransferFilesEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWTRANSFERFILESEVENT_H
