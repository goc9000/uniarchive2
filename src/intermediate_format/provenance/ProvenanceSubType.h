/**
 * intermediate_format/provenance/ProvenanceSubType.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 65304d8c06025bb089bc1fc5a6389f2b11352d7847d7f3c9545a340f31468808
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCESUBTYPE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCESUBTYPE_H

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

enum class ProvenanceSubType : uint8_t {
    ARCHIVE_FILE,
    ADIUM_ARCHIVE_FILE,
    EVENT_RANGE,
    MSN_CONVERSATION,
    SKYPE_CONVERSATION,
};

QString name_for_provenance_sub_type(ProvenanceSubType type);

QDebug operator<< (QDebug stream, ProvenanceSubType type);

QDataStream& operator>> (QDataStream& mut_stream, ProvenanceSubType& mut_type);
QDataStream& operator<< (QDataStream& mut_stream, ProvenanceSubType type);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCESUBTYPE_H
