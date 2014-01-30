#include "RawConversationFile.h"

#include <QFileInfo>
#include <QTextCodec>


RawConversationFile::RawConversationFile(QString filePath, QString fullPathInfo)
{
    filePath_ = filePath;
    fullPathInfo_ = fullPathInfo;
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
    return QFileInfo(filePath_).baseName();
}

QString RawConversationFile::fullPathInfo() const
{
    return fullPathInfo_;
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
    return openUtf8()->readAll();
}

QString RawConversationFile::description() const
{
    return filePath_;
}
