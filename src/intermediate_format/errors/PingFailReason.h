/**
 * intermediate_format/errors/PingFailReason.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_PINGFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_PINGFAILREASON_H

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class PingFailReason {
    UNDETERMINED = 0,
    BLOCKED_OR_UNSUPPORTED,
};

QString name_for_ping_fail_reason(PingFailReason protocol);

QDebug operator<< (QDebug stream, PingFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_PINGFAILREASON_H
