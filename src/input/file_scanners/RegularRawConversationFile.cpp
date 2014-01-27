#include "RegularRawConversationFile.h"

#include <QFileInfo>


RegularRawConversationFile::RegularRawConversationFile(QString filename)
    : RawConversationFile(filename,
                          RegularRawConversationFile::getFullPathInfo(filename))
{
}

QString RegularRawConversationFile::getFullPathInfo(QString filename)
{
    return QFileInfo(filename).canonicalFilePath();
}
