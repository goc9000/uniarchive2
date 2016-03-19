#include "TextBasedDecoder.h"

#include <QTextCodec>

#include "utils/fail.h"


void TextBasedDecoder::_startReading()
{
    reader_ = convFile_->openUtf8();
    nextLineNo_ = 1;
    nextLineBuffered_ = false;
}

QString TextBasedDecoder::_readNextLine()
{
    QString line = nextLineBuffered_ ? nextLine_ : reader_->readLine();
    nextLineBuffered_ = false;

    switch (reader_->status()) {
    case QTextStream::Ok:
        nextLineNo_ += 1;
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

QString TextBasedDecoder::_peekNextLine()
{
    if (!nextLineBuffered_) {
        nextLine_ = reader_->readLine();
        nextLineBuffered_ = true;
    }

    return nextLine_;
}
