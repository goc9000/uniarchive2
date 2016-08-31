/**
 * intermediate_format/events/file_transfer/RawReceiveFileEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): c6952b5606a3b457ac8c44faac38f60b4eb32fcae80943964de1f6e911a3768d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawReceiveFileEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawReceiveFileEvent::RawReceiveFileEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) receiver,
    IMM(RawTransferredFile) file
) : RawEvent(timestamp, index), receiver(move(receiver)), file(file) {
    sanityCheckMandatoryParameters();
}

void RawReceiveFileEvent::sanityCheckMandatoryParameters() const {
    invariant(receiver, "Parameter 'receiver' cannot have empty value");
}

QString RawReceiveFileEvent::eventName() const {
    return "ReceiveFile";
}

void RawReceiveFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " receiver=" << receiver.get();
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << file;
}

}}}
