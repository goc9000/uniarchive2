/**
 * intermediate_format/content/symbols/SkypeEmoticon.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): b0d86f982a2ee226513192a9ccc192462a45fbd5191016c314842059c2083edc
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/symbols/SkypeEmoticon.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

SkypeEmoticon::SkypeEmoticon(IMM(QString) semantics, IMM(QString) text_equivalent)
  : RawMessageContentItem(), semantics(semantics), textEquivalent(text_equivalent) {
}

RawMessageContentItemSubType SkypeEmoticon::subType() const {
    return RawMessageContentItemSubType::SKYPE_EMOTICON;
}

CEDE(SkypeEmoticon) SkypeEmoticon::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::SKYPE_EMOTICON);

    QString semantics = must_deserialize(mut_stream, QString);
    QString text_equivalent = must_deserialize(mut_stream, QString);

    unique_ptr<SkypeEmoticon> item = make_unique<SkypeEmoticon>(semantics, text_equivalent);

    return item;
}

void SkypeEmoticon::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << semantics << textEquivalent;
}

void SkypeEmoticon::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[SkypeEmoticon";
    stream << " semantics=" << semantics << " text_equivalent=" << textEquivalent;
    stream << "]";
}

}}}
