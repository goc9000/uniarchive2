#ifndef RAWCONVERSATIONFILE_H
#define RAWCONVERSATIONFILE_H

#include <memory>

#include <QString>


class RawConversationFile
{
public:
    RawConversationFile(QString filePath, QString fullPathInfo);

    QString filePath() const;
    QString filename() const;
    QString basename() const;

    QString fullPathInfo() const;

    QString description() const;

protected:
    QString filePath_;
    QString fullPathInfo_;
};

typedef std::unique_ptr<RawConversationFile> RawConversationFileUqPtr;

#endif // RAWCONVERSATIONFILE_H
