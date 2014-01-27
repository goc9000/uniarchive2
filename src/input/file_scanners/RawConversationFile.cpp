#include "RawConversationFile.h"


RawConversationFile::RawConversationFile(QString filename, QString fullPathInfo)
{
    filename_ = filename;
    fullPathInfo_ = fullPathInfo;
}

QString RawConversationFile::filename() const
{
    return filename_;
}

QString RawConversationFile::fullPathInfo() const
{
    return fullPathInfo_;
}

QString RawConversationFile::description() const
{
    return filename_;
}
