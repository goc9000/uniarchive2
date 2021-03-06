/**
 * intermediate_format/provenance/ProvenanceSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): ed7df770ffd62f3900b39b4c13701c83760224fe492e0e15980c5ad43516261a
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

#include <QMap>
#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace uniarchive2::utils::serialization;

QString name_for_provenance_sub_type(ProvenanceSubType type) {
    switch (type) {
        case ProvenanceSubType::ARCHIVE_ENTRY:
            return "ArchiveEntry";
        case ProvenanceSubType::ARCHIVE_FILE:
            return "ArchiveFile";
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
            return "AdiumArchiveFile";
        case ProvenanceSubType::EVENT_RANGE:
            return "EventRange";
        case ProvenanceSubType::FILE:
            return "File";
        case ProvenanceSubType::MSN_CONVERSATION:
            return "MSNConversation";
        case ProvenanceSubType::SKYPE_CONVERSATION:
            return "SkypeConversation";
    }

    invariant_violation("Invalid ProvenanceSubType value (%lld)", (int64_t)type);
}

QString symbol_for_provenance_sub_type(ProvenanceSubType type) {
    switch (type) {
        case ProvenanceSubType::ARCHIVE_ENTRY:
            return "archive_entry";
        case ProvenanceSubType::ARCHIVE_FILE:
            return "archive_file";
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
            return "adium_archive_file";
        case ProvenanceSubType::EVENT_RANGE:
            return "event_range";
        case ProvenanceSubType::FILE:
            return "file";
        case ProvenanceSubType::MSN_CONVERSATION:
            return "msn_conversation";
        case ProvenanceSubType::SKYPE_CONVERSATION:
            return "skype_conversation";
    }

    invariant_violation("Invalid ProvenanceSubType value (%lld)", (int64_t)type);
}

ProvenanceSubType provenance_sub_type_from_symbol(IMM(QString) symbol) {
    const static QMap<QString, ProvenanceSubType> MAP = {
        { "archive_entry",      ProvenanceSubType::ARCHIVE_ENTRY },
        { "archive_file",       ProvenanceSubType::ARCHIVE_FILE },
        { "adium_archive_file", ProvenanceSubType::ADIUM_ARCHIVE_FILE },
        { "event_range",        ProvenanceSubType::EVENT_RANGE },
        { "file",               ProvenanceSubType::FILE },
        { "msn_conversation",   ProvenanceSubType::MSN_CONVERSATION },
        { "skype_conversation", ProvenanceSubType::SKYPE_CONVERSATION },
    };

    if (MAP.contains(symbol)) {
        return MAP[symbol];
    }

    invariant_violation("Invalid ProvenanceSubType symbol ('%s')", QP(symbol));
}

QDebug operator<< (QDebug stream, ProvenanceSubType type) {
    stream << QP(name_for_provenance_sub_type(type));

    return stream;
}

QDataStream& operator>> (QDataStream& mut_stream, ProvenanceSubType& mut_type) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((ProvenanceSubType)code) {
        case ProvenanceSubType::ARCHIVE_ENTRY:
        case ProvenanceSubType::ARCHIVE_FILE:
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
        case ProvenanceSubType::EVENT_RANGE:
        case ProvenanceSubType::FILE:
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
