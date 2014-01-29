#ifndef RAWCONVERSATIONFILE_H
#define RAWCONVERSATIONFILE_H

#include <memory>

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

typedef std::unique_ptr<RawConversationFile> RawConversationFileUqPtr;

#endif // RAWCONVERSATIONFILE_H
