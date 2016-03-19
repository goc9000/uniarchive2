#include "RegularRawConversationFile.h"

#include <QFileInfo>

#include "utils/fail.h"


RegularRawConversationFile::RegularRawConversationFile(QString filename)
    : RawConversationFile(filename)
{
    fullPathInfo_ = RegularRawConversationFile::getFullPathInfo(filename);
    lastChangeDate_ =
        TimeStamp::fromLocalTime(QFileInfo(filename).lastModified());
}

QIODeviceUqPtr RegularRawConversationFile::openBinary() const
{
    QFileUqPtr file(new QFile(filePath_));

    if (!file->open(QIODevice::ReadOnly)) {
        fail("Could not open file '%s'", qPrintable(description()));
    }

    return QIODeviceUqPtr(file.release());
}

QString RegularRawConversationFile::getFullPathInfo(QString filename)
{
    return QFileInfo(filename).canonicalFilePath();
}
