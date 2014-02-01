#include "RawConversationFile.h"

#include <QFileInfo>
#include <QTextCodec>

#include "utils/fail.h"


RawConversationFile::RawConversationFile(QString filePath,
                                         QString fullPathInfo,
                                         TimeStamp lastChangeDate)
{
    filePath_ = filePath;
    fullPathInfo_ = fullPathInfo;
    lastChangeDate_ = lastChangeDate;
}

RawConversationFile::~RawConversationFile()
{
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
    return QFileInfo(filePath_).completeBaseName();
}

QString RawConversationFile::fullPathInfo() const
{
    return fullPathInfo_;
}

TimeStamp RawConversationFile::lastChangeDate() const
{
    return lastChangeDate_;
}

QTextStreamUqPtr RawConversationFile::openUtf8()
{
    QTextStreamUqPtr reader(new QOwningTextStream(openBinary().release()));

    reader->setCodec(QTextCodec::codecForName("UTF-8"));

    return reader;
}

QByteArray RawConversationFile::allData()
{
    return openBinary()->readAll();
}

QString RawConversationFile::allUtf8Text()
{
    QTextStreamUqPtr reader = openUtf8();

    QString text = reader->readAll();

    if (reader->status() != QTextStream::Ok) {
        fail("Cannot read file as UTF-8 text");
    }

    return text;
}

QString RawConversationFile::description() const
{
    return filePath_;
}
