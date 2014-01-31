#ifndef TEXTBASEDDECODER_H
#define TEXTBASEDDECODER_H

#include "decoders/RawConversationDecoder.h"


class TextBasedDecoder : public RawConversationDecoder
{
public:
    using RawConversationDecoder::RawConversationDecoder;

protected:
    void _startReading();
    QString _readNextLine();

    QTextStreamUqPtr reader_;
    int currentLineNo_;
};

#endif // TEXTBASEDDECODER_H
