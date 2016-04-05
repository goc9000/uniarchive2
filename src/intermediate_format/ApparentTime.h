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
#include <QDate>
#include <QTime>

#include "utils/external_libs/optional.hpp"

using namespace std::experimental;

namespace uniarchive2 { namespace intermediate_format {

struct ApparentTime {
public:
    enum class Reference {
        UNKNOWN,
        LOCAL_TIME, // The local time when the conversation was recorded
        UTC,
    };

    optional<QDate> date;
    optional<QTime> time;
    bool secondsSpecified;
    Reference reference;

    ApparentTime();
    ApparentTime(quint32 unix_timestamp, Reference reference);

    bool isSpecified() const;
    bool hasSpecifiedDate() const;
    bool hasSpecifiedTime() const;
    bool hasSpecifiedSeconds() const;
};

QDebug operator<< (QDebug stream, const ApparentTime& time);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_APPARENTTIME_H
