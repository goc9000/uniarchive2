/**
 * intermediate_format/provenance/ProvenanceSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 56c759228ee7387e27424ca7153355d1b1c0cdf72fca354f138fa792c3111771
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/ProvenanceSubType.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

QString name_for_provenance_sub_type(ProvenanceSubType type) {
    switch (type) {
        case ProvenanceSubType::ARCHIVE_FILE:
            return "archive file";
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
            return "Adium archive file";
        case ProvenanceSubType::EVENT_RANGE:
            return "event range";
        case ProvenanceSubType::MSN_CONVERSATION:
            return "MSN conversation";
        case ProvenanceSubType::SKYPE_CONVERSATION:
            return "Skype conversation";
    }

    invariant_violation("Invalid ProvenanceSubType value (%d)", type);
}

QDebug operator<< (QDebug stream, ProvenanceSubType type) {
    stream << QP(name_for_provenance_sub_type(type));

    return stream;
}

QDataStream& operator<< (QDataStream& mut_stream, ProvenanceSubType type) {
    mut_stream << (quint32)type;

    return mut_stream;
}

}}}