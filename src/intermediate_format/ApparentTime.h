/**
 * intermediate_format/ApparentTime.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_APPARENTTIME_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_APPARENTTIME_H

#include <QtDebug>
#include <QByteArray>
#include <QDate>
#include <QString>
#include <QTime>
#include <QDateTime>

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

using namespace std::experimental;

namespace uniarchive2 { namespace intermediate_format {

struct ApparentTime {
public:
    enum class Reference {
        UNKNOWN,
        LOCAL_TIME, // The local time when the conversation was recorded
        OFFSET_FROM_UTC,
        TIMEZONE
    };

    optional<QDate> date;
    optional<QTime> time;
    bool secondsSpecified;
    Reference reference;
    int utcOffsetQuarters;
    QByteArray timeZoneID;

    ApparentTime();
    static ApparentTime fromUnixTimestamp(quint32 unix_timestamp);
    static ApparentTime fromQDateTime(IMM(QDateTime) datetime);
    static ApparentTime fromQDateTimeUnknownReference(IMM(QDateTime) datetime);

    bool isSpecified() const;
    bool hasSpecifiedDate() const;
    bool hasSpecifiedTime() const;
    bool hasSpecifiedSeconds() const;

    QString timeZoneName() const;
};

QDebug operator<< (QDebug stream, IMM(ApparentTime) time);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_APPARENTTIME_H
