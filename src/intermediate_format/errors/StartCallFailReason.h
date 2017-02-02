/**
 * intermediate_format/errors/StartCallFailReason.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e79b1599ab16c014c81e82fa7878c683bcca478ccdb9a6da23a02c45e22fc65f
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_STARTCALLFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_STARTCALLFAILREASON_H

#include "utils/language/shortcuts.h"

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class StartCallFailReason : uint8_t {
    UNDETERMINED = 0,
    NO_ANSWER,
    PEER_BUSY,
    PRIVACY_BLOCKED,
    CALL_REJECTED,
    INSUFFICIENT_FUNDS,
};

QString name_for_start_call_fail_reason(StartCallFailReason reason);

QString symbol_for_start_call_fail_reason(StartCallFailReason reason);
StartCallFailReason start_call_fail_reason_from_symbol(IMM(QString) symbol);

QDebug operator<< (QDebug stream, StartCallFailReason reason);

QDataStream& operator>> (QDataStream& mut_stream, StartCallFailReason& mut_reason);
QDataStream& operator<< (QDataStream& mut_stream, StartCallFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_STARTCALLFAILREASON_H
