#include "TimeStamp.h"


bool TimeStamp::isNull() const
{
    return date.isNull() && time.isNull() && timeZone.isNull();
}

bool TimeStamp::hasTimeZoneInfo() const
{
    return !timeZone.isNull();
}

bool TimeStamp::operator ==(const TimeStamp& other) const
{
    return (compare(other) == 0);
}

bool TimeStamp::operator <(const TimeStamp& other) const
{
    return (compare(other) < 0);
}

bool TimeStamp::operator <=(const TimeStamp& other) const
{
    return (compare(other) <= 0);
}

bool TimeStamp::operator >(const TimeStamp& other) const
{
    return (compare(other) > 0);
}

bool TimeStamp::operator >=(const TimeStamp& other) const
{
    return (compare(other) >= 0);
}

int TimeStamp::compare(const TimeStamp& other) const
{
    return
}

QString TimeStamp::description() const
{
    QString desc;

    if (isNull()) {
        return "<no date>";
    }

    if (!date.isNull()) {
        desc.append(date.toString("yyyy-MM-dd"));
    }
    if (!time.isNull()) {
        if (desc != "") {
            desc.append(' ');
        }
        desc.append(time.toString("hh:mm:ss"));
    }
    if (!timeZone.isNull() && (timeZone.description() != "")) {
        if (desc != "") {
            desc.append(' ');
        }
        desc.append(timeZone.description());
    }

    return desc;
}

void TimeStamp::setNull()
{
    date = QDate();
    time = QTime();
    timeZone.setNull();
}

TimeStamp TimeStamp::fromLocalTime(QDateTime localTime)
{
    QDateTime absoluteTime = localTime.toUTC();

    return TimeStamp(absoluteTime.date(),
                     absoluteTime.time(),
                     TimeZoneInfo::UTC());
}
