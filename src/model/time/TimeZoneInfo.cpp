#include "TimeZoneInfo.h"

#include <QRegExp>

#include "utils/fail.h"


bool TimeZoneInfo::isNull() const
{
    return timeZoneName.isNull();
}

bool TimeZoneInfo::isUtc() const
{
    return !isNull() && !offsetToUtc;
}

QString TimeZoneInfo::description() const
{
    if (isNull()) {
        return "";
    }
    if (isUtc()) {
        return "UTC";
    }

    return QString().sprintf("%c%02d%02d",
                             offsetToUtc >= 0 ? '+' : '-',
                             abs(offsetToUtc) / 60,
                             abs(offsetToUtc) % 60) +
           timeZoneName;
}

void TimeZoneInfo::setNull()
{
    offsetToUtc = 0;
    timeZoneName = QString::Null();
}

TimeZoneInfo TimeZoneInfo::UTC()
{
    return TimeZoneInfo(0, "UTC");
}

TimeZoneInfo TimeZoneInfo::fromString(QString tzText)
{
    static QRegExp PAT_TIMEZONE(R"(^([+-])(\d\d)(\d\d)(\w+)$)");

    if (tzText == "") {
        return TimeZoneInfo();
    }

    if (!PAT_TIMEZONE.exactMatch(tzText)) {
        fail("Invalid timezone format: '%s'", qPrintable(tzText));
    }

    QString sign = PAT_TIMEZONE.cap(1);
    int offsetHours = PAT_TIMEZONE.cap(2).toInt();
    int offsetMinutes = PAT_TIMEZONE.cap(3).toInt();
    QString timeZoneName = PAT_TIMEZONE.cap(4);

    if ((offsetHours > 12) || ((offsetMinutes != 0) && (offsetMinutes != 30))) {
        fail("Invalid timezone specification: '%s'", qPrintable(tzText));
    }

    return TimeZoneInfo(((sign == "-") ? -1 : 1) *
                        (offsetHours * 60 + offsetMinutes),
                        timeZoneName);
}
