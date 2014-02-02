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
    QString _peekNextLine();

    QTextStreamUqPtr reader_;
    int nextLineNo_;

    bool nextLineBuffered_;
    QString nextLine_;
};

#endif // TEXTBASEDDECODER_H
