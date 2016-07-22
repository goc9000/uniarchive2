/**
 * intermediate_format/events/file_transfer/RawTransferFilesEvent.h
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
#include "utils/language/shortcuts.h"

#include <QtDebug>

#include <vector>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;

struct RawTransferFilesEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> recipient;
    vector<RawTransferredFile> files;

    RawTransferFilesEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) sender,
        IMM(vector<RawTransferredFile>) files
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWTRANSFERFILESEVENT_H
