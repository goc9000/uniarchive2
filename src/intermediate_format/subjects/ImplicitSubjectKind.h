/**
 * intermediate_format/subjects/ImplicitSubjectKind.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 63f4012ca8d693b9d1116e237865c9f0602ffaec5e717965bb5bec12664aad1a
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECTKIND_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECTKIND_H

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

enum class ImplicitSubjectKind : uint8_t {
    INVALID,
    IDENTITY, // Whoever the identity (i.e. local user) is
    FILE_RECEIVER,
};

QString name_for_implicit_subject_kind(ImplicitSubjectKind kind);

QDebug operator<< (QDebug stream, ImplicitSubjectKind kind);

QDataStream& operator>> (QDataStream& mut_stream, ImplicitSubjectKind& mut_kind);
QDataStream& operator<< (QDataStream& mut_stream, ImplicitSubjectKind kind);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_SUBJECTS_IMPLICITSUBJECTKIND_H
