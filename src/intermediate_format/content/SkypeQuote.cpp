/**
 * intermediate_format/content/SkypeQuote.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): adbdaff928545fa55c45e14a88bfe467798800e9cab74e9cc43748a8bb99da8a
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/SkypeQuote.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

SkypeQuote::SkypeQuote(
    IMM(QString) conversation_id,
    IMM(QString) message_guid,
    TAKE(ApparentSubject) author,
    IMM(ApparentTime) time,
    IMM(QString) legacy_header,
    RawMessageContent&& content
) : RawMessageContentItem(), quotedMessageConversationID(conversation_id), quotedMessageGUID(message_guid),
    quotedMessageAuthor(move(author)), quotedMessageTime(time), legacyHeader(legacy_header), content(move(content)) {
    sanityCheckMandatoryParameters();
}

void SkypeQuote::sanityCheckMandatoryParameters() const {
    invariant(quotedMessageAuthor, "Parameter 'author' cannot have empty value");
}

RawMessageContentItemSubType SkypeQuote::subType() const {
    return RawMessageContentItemSubType::SKYPE_QUOTE;
}

bool SkypeQuote::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const {
    return visit_subjects(quotedMessageAuthor, callback) && visit_subjects(content, callback);
}

bool SkypeQuote::alterSubjectsImpl(IMM(alter_subjects_callback_t) callback) {
    return alter_subjects(quotedMessageAuthor, callback) && alter_subjects(content, callback);
}

CEDE(SkypeQuote) SkypeQuote::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::SKYPE_QUOTE);

    QString conversation_id = must_deserialize(mut_stream, QString);
    QString message_guid = must_deserialize(mut_stream, QString);
    unique_ptr<ApparentSubject> author = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    ApparentTime time = must_deserialize(mut_stream, ApparentTime);
    QString legacy_header = must_deserialize(mut_stream, QString);
    RawMessageContent content = must_deserialize(mut_stream, RawMessageContent);

    unique_ptr<SkypeQuote> item = make_unique<SkypeQuote>(
        conversation_id,
        message_guid,
        move(author),
        time,
        legacy_header,
        move(content)
    );

    return item;
}

void SkypeQuote::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << quotedMessageConversationID << quotedMessageGUID << quotedMessageAuthor << quotedMessageTime;
    mut_stream << legacyHeader << content;
}

void SkypeQuote::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[SkypeQuote";
    stream << " conversation_id=" << quotedMessageConversationID << " message_guid=" << quotedMessageGUID;
    stream << " author=" << quotedMessageAuthor << " time=" << quotedMessageTime << " legacy_header=" << legacyHeader;
    stream << " content=" << content;
    stream << "]";
}

}}}
