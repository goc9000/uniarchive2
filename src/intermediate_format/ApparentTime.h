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

#include "intermediate_format/ApparentTimeReference.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"
#include "utils/serialization/ISerializable.h"

#include <QDebug>
#include <QByteArray>
#include <QDate>
#include <QString>
#include <QTime>
#include <QDateTime>

namespace uniarchive2 { namespace intermediate_format {

using namespace std::experimental;
using namespace uniarchive2::utils::serialization;

struct ApparentTime : ISerializable {
public:
    // These three fields form the "mantissa" of the date, i.e. the day/hour/etc. numbers relative to a reference
    optional<QDate> date;
    optional<QTime> time;
    bool secondsSpecified = true;

    ApparentTimeReference reference = ApparentTimeReference::UNKNOWN;

    // These fields describe the time reference, if reference=SPECIFIED. They are listed in increasing order of
    // specificity: if a timezone abbreviation is specified, the UTC offset is implied (but will be stored nonetheless
    // if provided, to enable sanity checks), just as a timezone abbreviation is implied by the full timezone ID in
    // the context of the given date
    optional<int> utcOffsetQuarters; // The UTC offset for the timezone, valid at the given date
    QString timeZoneAbbreviation; // The abbreviation for the timezone, valid at the given date
    QByteArray timeZoneID; // The timezone ID

    ApparentTime();
    static ApparentTime fromUnixTimestamp(quint32 unix_timestamp);
    static ApparentTime fromQDateTime(IMM(QDateTime) datetime);
    static ApparentTime fromQDateTimeLocalTime(IMM(QDateTime) datetime);
    static ApparentTime fromQDateTimeUnknownReference(IMM(QDateTime) datetime);

    bool isSpecified() const;
    bool hasSpecifiedDate() const;
    bool hasSpecifiedTime() const;
    bool hasSpecifiedSeconds() const;

    void serializeToStream(QDataStream& mut_stream) const;
};

QDebug operator<< (QDebug stream, IMM(ApparentTime) time);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_APPARENTTIME_H
