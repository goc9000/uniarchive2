#ifndef PIDGINTEXTFORMATDECODER_H
#define PIDGINTEXTFORMATDECODER_H

#include "decoders/RawConversationDecoder.h"


class PidginTextFormatDecoder : public RawConversationDecoder
{
public:
    using RawConversationDecoder::RawConversationDecoder;

    std::vector<RawConversation> rawConversations();

    static bool recognize(RawConversationFile *convFile);
};

#endif // PIDGINTEXTFORMATDECODER_H
