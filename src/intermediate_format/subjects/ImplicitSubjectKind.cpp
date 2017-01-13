/**
 * intermediate_format/subjects/ImplicitSubjectKind.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 461fff185ec9b27db41e23dc357874f4eb0ba6bbcf5a1168ac4f909ac0648ece
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/ImplicitSubjectKind.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QMap>
#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

using namespace uniarchive2::utils::serialization;

QString name_for_implicit_subject_kind(ImplicitSubjectKind kind) {
    switch (kind) {
        case ImplicitSubjectKind::INVALID:
            return "(invalid)";
        case ImplicitSubjectKind::IDENTITY:
            return "identity";
        case ImplicitSubjectKind::FILE_RECEIVER:
            return "file receiver";
    }

    invariant_violation("Invalid ImplicitSubjectKind value (%lld)", (int64_t)kind);
}

QString symbol_for_implicit_subject_kind(ImplicitSubjectKind kind) {
    switch (kind) {
        case ImplicitSubjectKind::INVALID:
            return "invalid";
        case ImplicitSubjectKind::IDENTITY:
            return "identity";
        case ImplicitSubjectKind::FILE_RECEIVER:
            return "file_receiver";
    }

    invariant_violation("Invalid ImplicitSubjectKind value (%lld)", (int64_t)kind);
}

ImplicitSubjectKind implicit_subject_kind_from_symbol(IMM(QString) symbol) {
    const static QMap<QString, ImplicitSubjectKind> MAP = {
        { "invalid",       ImplicitSubjectKind::INVALID },
        { "identity",      ImplicitSubjectKind::IDENTITY },
        { "file_receiver", ImplicitSubjectKind::FILE_RECEIVER },
    };

    if (MAP.contains(symbol)) {
        return MAP[symbol];
    }

    invariant_violation("Invalid ImplicitSubjectKind symbol ('%s')", QP(symbol));
}

QDebug operator<< (QDebug stream, ImplicitSubjectKind kind) {
    stream << QP(name_for_implicit_subject_kind(kind));

    return stream;
}

QDataStream& operator>> (QDataStream& mut_stream, ImplicitSubjectKind& mut_kind) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((ImplicitSubjectKind)code) {
        case ImplicitSubjectKind::INVALID:
        case ImplicitSubjectKind::IDENTITY:
        case ImplicitSubjectKind::FILE_RECEIVER:
            mut_kind = (ImplicitSubjectKind)code;
            break;
        default:
            invariant_violation("Invalid deserialized ImplicitSubjectKind value (%lld)", (int64_t)code);
    }

    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, ImplicitSubjectKind kind) {
    mut_stream << (uint8_t)kind;

    return mut_stream;
}

}}}
