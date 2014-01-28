#ifndef RAWCONVERSATIONDECODER_H
#define RAWCONVERSATIONDECODER_H

#include <vector>

#include "input/file_scanners/RawConversationFile.h"
#include "model/raw_conversation/RawConversation.h"


class RawConversationDecoder
{
public:
    RawConversationDecoder(RawConversationFile *convFile);
    virtual ~RawConversationDecoder();

    virtual std::vector<RawConversation> rawConversations() = 0;

    static bool isSupportedFormat(RawConversationFile *convFile);
    static RawConversationDecoder* forRawConversationFile(
        RawConversationFile *convFile);

protected:
    RawConversationFile *convFile_;
};

#endif // RAWCONVERSATIONDECODER_H
