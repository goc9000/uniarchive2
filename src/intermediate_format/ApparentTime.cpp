/**
 * intermediate_format/ApparentTime.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDateTime>
#include <QDebugStateSaver>

#include "intermediate_format/ApparentTime.h"

namespace uniarchive2 { namespace intermediate_format {

ApparentTime::ApparentTime(
): date(), time(), secondsSpecified(false), reference(Reference::UNKNOWN) {
}

ApparentTime::ApparentTime(quint32 unix_timestamp, Reference reference)
: secondsSpecified(true), reference(reference) {
    QDateTime timestamp = QDateTime::fromTime_t(unix_timestamp, Qt::UTC);
    date = timestamp.date();
    time = timestamp.time();
}

bool ApparentTime::isSpecified() const {
    return hasSpecifiedDate() || hasSpecifiedTime();
}

bool ApparentTime::hasSpecifiedDate() const {
    return (bool)date;
}

bool ApparentTime::hasSpecifiedTime() const {
    return (bool)time;
}

bool ApparentTime::hasSpecifiedSeconds() const {
    return hasSpecifiedTime() || secondsSpecified;
}

QDebug operator<< (QDebug stream, const ApparentTime& time) {
    QDebugStateSaver settings(stream);
    stream.nospace();

    if (!time.isSpecified()) {
        stream << "(unspecified time)";
        return stream;
    }

    if (time.hasSpecifiedDate()) {
        stream << qPrintable(time.date->toString("yyyy-MM-dd"));
        if (time.hasSpecifiedTime()) {
            stream << " ";
        }
    }
    if (time.hasSpecifiedTime()) {
        stream << qPrintable(time.time->toString(time.hasSpecifiedSeconds() ? "HH:mm:ss" : "HH:mm"));
    }
    switch (time.reference) {
        case ApparentTime::Reference::UNKNOWN:
            stream << " (unknown reference)";
            break;
        case ApparentTime::Reference::LOCAL_TIME:
            stream << " (local time)";
            break;
        case ApparentTime::Reference::UTC:
            stream << " UTC";
            break;
    }

    return stream;
}

}}