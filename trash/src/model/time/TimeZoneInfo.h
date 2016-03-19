#ifndef TIMEZONEINFO_H
#define TIMEZONEINFO_H

#include <QString>


class TimeZoneInfo
{
public:
    TimeZoneInfo()
        : offsetToUtc(0), timeZoneName(QString::Null()) {}
    TimeZoneInfo(int offsetToUtc, QString timeZoneName="")
        : offsetToUtc(offsetToUtc), timeZoneName(timeZoneName) {}

    bool isNull() const;
    bool isUtc() const;

    QString description() const;

    void setNull();

    int offsetToUtc; // in minutes
    QString timeZoneName;

    static TimeZoneInfo UTC();

    static TimeZoneInfo fromString(QString tzText);
};

#endif // TIMEZONEINFO_H
