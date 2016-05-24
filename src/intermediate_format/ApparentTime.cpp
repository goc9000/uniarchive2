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
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format {

QString format_utc_offset(int offset_quarters);

ApparentTime::ApparentTime() {
}

ApparentTime ApparentTime::fromUnixTimestamp(quint32 unix_timestamp) {
    QDateTime timestamp = QDateTime::fromTime_t(unix_timestamp, Qt::UTC);

    ApparentTime time;
    time.date = timestamp.date();
    time.time = timestamp.time();
    time.reference = Reference::SPECIFIED;
    time.timeZoneID = QTimeZone::utc().id();

    return time;
}

ApparentTime ApparentTime::fromQDateTime(IMM(QDateTime) datetime) {
    ApparentTime time;
    time.date = datetime.date();
    time.time = datetime.time();

    switch (datetime.timeSpec()) {
        case Qt::LocalTime:
            invariant_violation("Cannot initialize equivalent ApparentTime for QDateTime defined on local timespec");
        case Qt::UTC:
            time.reference = Reference::SPECIFIED;
            time.timeZoneID = QTimeZone::utc().id();
            break;
        case Qt::OffsetFromUTC:
            time.reference = Reference::SPECIFIED;
            invariant(abs(datetime.offsetFromUtc()) % 900 == 0, "UTC offset must be a multiple of 15 minutes");
            time.utcOffsetQuarters = datetime.offsetFromUtc() / 900;
            break;
        case Qt::TimeZone:
            time.reference = Reference::SPECIFIED;
            time.timeZoneID = datetime.timeZone().id();
            break;
    }

    return time;
}

ApparentTime ApparentTime::fromQDateTimeUnknownReference(IMM(QDateTime) datetime) {
    ApparentTime time;
    time.date = datetime.date();
    time.time = datetime.time();
    time.reference = Reference::UNKNOWN;

    return time;
}

ApparentTime ApparentTime::fromQDateTimeLocalTime(IMM(QDateTime) datetime) {
    ApparentTime time;
    time.date = datetime.date();
    time.time = datetime.time();
    time.reference = Reference::LOCAL_TIME;

    return time;
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
    return hasSpecifiedTime() && secondsSpecified;
}

QDebug operator<< (QDebug stream, IMM(ApparentTime) time) {
    QDebugStateSaver settings(stream);
    stream.nospace();

    if (!time.isSpecified()) {
        stream << "(unspecified time)";
        return stream;
    }

    if (time.hasSpecifiedDate()) {
        stream << QP(time.date->toString("yyyy-MM-dd"));
        if (time.hasSpecifiedTime()) {
            stream << " ";
        }
    }
    if (time.hasSpecifiedTime()) {
        stream << QP(time.time->toString(time.hasSpecifiedSeconds() ? "HH:mm:ss" : "HH:mm"));
    }
    switch (time.reference) {
        case ApparentTime::Reference::UNKNOWN:
            stream << " (unknown reference)";
            break;
        case ApparentTime::Reference::LOCAL_TIME:
            stream << " (local time)";
            break;
        case ApparentTime::Reference::SPECIFIED:
            if (time.utcOffsetQuarters) {
                stream << QP(format_utc_offset(*time.utcOffsetQuarters));
            }
            if (!time.timeZoneAbbreviation.isEmpty()) {
                stream << " " << QP(time.timeZoneAbbreviation);
            }
            if (!time.timeZoneID.isEmpty()) {
                QTimeZone timeZone(time.timeZoneID);
                QDateTime datetime(*time.date, *time.time, timeZone);

                stream << " ";
                if (!time.timeZoneAbbreviation.isEmpty()) {
                    stream << "(";
                }
                stream << QP(timeZone.displayName(datetime, QTimeZone::ShortName));
                if (!time.timeZoneAbbreviation.isEmpty()) {
                    stream << ")";
                }
            }
            break;
    }

    return stream;
}

QString format_utc_offset(int offset_quarters) {
    if (!offset_quarters) {
        return "Z";
    }

    return QString("%1%2:%3")
        .arg(offset_quarters < 0 ? "-" : "+")
        .arg(abs(offset_quarters) / 4, 2, 10, QChar('0'))
        .arg((abs(offset_quarters) % 4) * 15, 2, 10, QChar('0'));
}

}}