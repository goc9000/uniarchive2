#ifndef RAWCONVERSATIONDECODER_H
#define RAWCONVERSATIONDECODER_H

#include <vector>
#include <memory>

#include "input/file_scanners/RawConversationFile.h"
#include "model/raw_conversation/RawConversation.h"


class RawConversationDecoder;
typedef std::unique_ptr<RawConversationDecoder> RawConversationDecoderUqPtr;


class RawConversationDecoder
{
public:
    RawConversationDecoder(RawConversationFile* convFile);
    virtual ~RawConversationDecoder();

    virtual std::vector<RawConversation> rawConversations() = 0;

    static bool isSupportedFormat(RawConversationFile* convFile);
    static RawConversationDecoderUqPtr
        forRawConversationFile(RawConversationFile* convFile);

protected:
    RawConversationFile* convFile_;
};

#endif // RAWCONVERSATIONDECODER_H
