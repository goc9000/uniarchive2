/**
 * intermediate_format/events/file_transfer/RawOfferFileEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3d1a011e0efd6b82704c72845f94315e634438689527c53056a0f013e9793a97
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawOfferFileEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawOfferFileEvent::RawOfferFileEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) sender,
    IMM(RawTransferredFile) file
) : RawEvent(timestamp, index), sender(move(sender)), file(file) {
    sanityCheckMandatoryParameters();
}

void RawOfferFileEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
}

RawEventSubType RawOfferFileEvent::subType() const {
    return RawEventSubType::OFFER_FILE;
}

bool RawOfferFileEvent::visitSubjectsImpl(IApparentSubjectVisitor& visitor) {
    return visit_subjects(sender, visitor) && visit_subjects(recipient, visitor);
}

CEDE(RawOfferFileEvent) RawOfferFileEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::OFFER_FILE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> sender = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    unique_ptr<ApparentSubject> recipient = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);
    RawTransferredFile file = must_deserialize(mut_stream, RawTransferredFile);

    unique_ptr<RawOfferFileEvent> event = make_unique<RawOfferFileEvent>(timestamp, index, move(sender), file);
    event->skypeGUID = skype_guid;
    event->recipient = move(recipient);

    return event;
}

void RawOfferFileEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << sender;
    serialize_optional_unique_ptr(mut_stream, recipient);
    mut_stream << file;
}

void RawOfferFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender;
    if (recipient) {
        stream << " recipient=" << recipient;
    }
    stream << " file=" << file;
}

}}}
