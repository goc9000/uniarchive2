#include "TextBasedDecoder.h"

#include <QTextCodec>

#include "utils/fail.h"


void TextBasedDecoder::_startReading()
{
    reader_ = convFile_->openUtf8();
    currentLineNo_ = 0;
}

QString TextBasedDecoder::_readNextLine()
{
    QString line = reader_->readLine();

    switch (reader_->status()) {
    case QTextStream::Ok:
        return line;
    case QTextStream::ReadCorruptData:
        fail("Text file contains invalid characters for codec '%s'",
             reader_->codec()->name().data());
        break;
    case QTextStream::ReadPastEnd:
        fail("Read past end of stream");
        break;
    case QTextStream::WriteFailed:
        // should never happen
        break;
    }

    return QString::Null();
}
