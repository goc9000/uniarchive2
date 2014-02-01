#include "TimeStamp.h"


bool TimeStamp::isNull() const
{
    return date.isNull() && time.isNull() && timeZone.isNull();
}

bool TimeStamp::hasTimeZoneInfo() const
{
    return !timeZone.isNull();
}

QString TimeStamp::description() const
{
    QString desc;

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
