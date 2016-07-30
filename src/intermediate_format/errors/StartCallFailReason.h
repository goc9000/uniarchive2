/**
 * intermediate_format/errors/StartCallFailReason.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_STARTCALLFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_STARTCALLFAILREASON_H

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class StartCallFailReason {
    UNDETERMINED = 0,
    NO_ANSWER,
    BUSY,
    PRIVACY_BLOCKED,
    CALL_REJECTED,
};

QString name_for_start_call_fail_reason(StartCallFailReason protocol);

QDebug operator<< (QDebug stream, StartCallFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_STARTCALLFAILREASON_H
