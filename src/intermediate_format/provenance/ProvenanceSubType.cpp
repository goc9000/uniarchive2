/**
 * intermediate_format/provenance/ProvenanceSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 4f1bbb8f58b335d4bf00af734593fac610c5c130f515cc81f67b2a85afdc81b1
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/ProvenanceSubType.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace uniarchive2::utils::serialization;

QString name_for_provenance_sub_type(ProvenanceSubType type) {
    switch (type) {
        case ProvenanceSubType::ARCHIVE_FILE:
            return "ArchiveFile";
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
            return "AdiumArchiveFile";
        case ProvenanceSubType::EVENT_RANGE:
            return "EventRange";
        case ProvenanceSubType::MSN_CONVERSATION:
            return "MSNConversation";
        case ProvenanceSubType::SKYPE_CONVERSATION:
            return "SkypeConversation";
    }

    invariant_violation("Invalid ProvenanceSubType value (%lld)", (int64_t)type);
}

QDebug operator<< (QDebug stream, ProvenanceSubType type) {
    stream << QP(name_for_provenance_sub_type(type));

    return stream;
}

QDataStream& operator>> (QDataStream& mut_stream, ProvenanceSubType& mut_type) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((ProvenanceSubType)code) {
        case ProvenanceSubType::ARCHIVE_FILE:
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
        case ProvenanceSubType::EVENT_RANGE:
        case ProvenanceSubType::MSN_CONVERSATION:
        case ProvenanceSubType::SKYPE_CONVERSATION:
            mut_type = (ProvenanceSubType)code;
            break;
        default:
            invariant_violation("Invalid deserialized ProvenanceSubType value (%lld)", (int64_t)code);
    }

    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, ProvenanceSubType type) {
    mut_stream << (uint8_t)type;

    return mut_stream;
}

}}}
