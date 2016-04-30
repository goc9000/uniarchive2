/**
 * intermediate_format/ApparentTime.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <cmath>

#include <QDateTime>
#include <QDebugStateSaver>
#include <QTimeZone>

#include "intermediate_format/ApparentTime.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format {

QString format_utc_offset(int offset_quarters);

ApparentTime::ApparentTime(
): date(), time(), secondsSpecified(false), reference(Reference::UNKNOWN) {
}

ApparentTime::ApparentTime(quint32 unix_timestamp, Reference reference)
: secondsSpecified(true), reference(reference) {
    QDateTime timestamp = QDateTime::fromTime_t(unix_timestamp, Qt::UTC);
    date = timestamp.date();
    time = timestamp.time();
}

ApparentTime::ApparentTime(IMM(QDateTime) datetime): secondsSpecified(true) {
    date = datetime.date();
    time = datetime.time();

    switch (datetime.timeSpec()) {
        case Qt::LocalTime:
            invariant_violation("Cannot initialize equivalent ApparentTime for QDateTime defined on local timespec");
        case Qt::UTC:
            reference = Reference::UTC;
            break;
        case Qt::OffsetFromUTC:
            reference = Reference::OFFSET_FROM_UTC;
            invariant(abs(datetime.offsetFromUtc()) % 900 == 0, "UTC offset must be a multiple of 15 minutes");
            utcOffsetQuarters = datetime.offsetFromUtc() / 900;
            break;
        case Qt::TimeZone:
            reference = Reference::TIMEZONE;
            timeZoneID = datetime.timeZone().id();
            break;
    }
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

QString ApparentTime::timeZoneName() const {
    invariant(reference == Reference::TIMEZONE, "Cannot display time zone when reference is not TIMEZONE");

    QTimeZone timeZone(timeZoneID);
    invariant(timeZone.isValid(), "Time zone is invalid");

    QDateTime datetime(*date, *time, timeZone);
    return timeZone.displayName(datetime, QTimeZone::ShortName);
}

QDebug operator<< (QDebug stream, IMM(ApparentTime) time) {
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
        case ApparentTime::Reference::OFFSET_FROM_UTC:
            stream << " UTC" << qUtf8Printable(format_utc_offset(time.utcOffsetQuarters));
            break;
        case ApparentTime::Reference::TIMEZONE:
            stream << " (tz: " << qUtf8Printable(time.timeZoneName()) << ")";
            break;
    }

    return stream;
}

QString format_utc_offset(int offset_quarters) {
    return QString("%1%2:%3")
        .arg(offset_quarters < 0 ? "-" : "+")
        .arg(abs(offset_quarters) / 4, 2, 10, QChar('0'))
        .arg((abs(offset_quarters) % 4) * 15, 2, 10, QChar('0'));
}

}}