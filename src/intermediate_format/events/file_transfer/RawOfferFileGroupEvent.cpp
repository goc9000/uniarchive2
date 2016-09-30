/**
 * intermediate_format/events/file_transfer/RawOfferFileGroupEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 0ef2d45ca20f11aefb0d17e172e5f1eb34bf5b2e48180415dd1aded478c74a90
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawOfferFileGroupEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferFileGroupEvent::RawOfferFileGroupEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) sender,
    uint num_files
) : RawEvent(timestamp, index), sender(move(sender)), numFiles(num_files) {
    sanityCheckMandatoryParameters();
}

void RawOfferFileGroupEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
}

RawEventSubType RawOfferFileGroupEvent::subType() const {
    return RawEventSubType::OFFER_FILE_GROUP;
}

void RawOfferFileGroupEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get();
    if (recipient) {
        stream << " recipient=" << recipient.get();
    }
    stream << " num_files=" << numFiles;
}

}}}
