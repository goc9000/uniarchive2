#ifndef RAWCONVERSATIONFILE_H
#define RAWCONVERSATIONFILE_H

#include <QString>


class RawConversationFile
{
public:
    RawConversationFile(QString filename, QString fullPathInfo);

    QString filename() const;
    QString fullPathInfo() const;

    QString description() const;

protected:
    QString filename_;
    QString fullPathInfo_;
};

#endif // RAWCONVERSATIONFILE_H
