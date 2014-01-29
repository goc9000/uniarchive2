#include "RawConversationFile.h"

#include <QFileInfo>


RawConversationFile::RawConversationFile(QString filePath, QString fullPathInfo)
{
    filePath_ = filePath;
    fullPathInfo_ = fullPathInfo;
}

QString RawConversationFile::filePath() const
{
    return filePath_;
}

QString RawConversationFile::filename() const
{
    return QFileInfo(filePath_).fileName();
}

QString RawConversationFile::basename() const
{
    return QFileInfo(filePath_).baseName();
}

QString RawConversationFile::fullPathInfo() const
{
    return fullPathInfo_;
}

QString RawConversationFile::description() const
{
    return filePath_;
}
