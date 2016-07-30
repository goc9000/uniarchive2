/**
 * intermediate_format/errors/CurrentCallFailReason.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_CURRENTCALLFAILREASON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_CURRENTCALLFAILREASON_H

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

enum class CurrentCallFailReason {
    UNDETERMINED = 0,
    CONNECTION_DROPPED,
};

QString name_for_current_call_fail_reason(CurrentCallFailReason protocol);

QDebug operator<< (QDebug stream, CurrentCallFailReason reason);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_ERRORS_CURRENTCALLFAILREASON_H
