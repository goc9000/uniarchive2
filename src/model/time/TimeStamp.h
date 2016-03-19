#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QDate>
#include <QTime>
#include <QString>

#include "model/time/TimeZoneInfo.h"


class TimeStamp
{
public:
    TimeStamp() : date(), time(), timeZone() {}
    TimeStamp(QDate date, QTime time=QTime(),
              TimeZoneInfo timeZone=TimeZoneInfo())
        : date(date), time(time), timeZone(timeZone) {}
    TimeStamp(QTime time, TimeZoneInfo timeZone=TimeZoneInfo())
        : date(), time(time), timeZone(timeZone) {}

    bool isNull() const;
    bool hasTimeZoneInfo() const;

    bool operator ==(const TimeStamp& other) const;
    bool operator <(const TimeStamp& other) const;
    bool operator <=(const TimeStamp& other) const;
    bool operator >(const TimeStamp& other) const;
    bool operator >=(const TimeStamp& other) const;

    int compare(const TimeStamp& other) const;

    QString description() const;

    void setNull();

    QDate date;
    QTime time;
    TimeZoneInfo timeZone;

    static TimeStamp fromLocalTime(QDateTime localTime);
};

#endif // DATE_H
