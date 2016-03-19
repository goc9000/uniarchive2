#include "RawConversationDecoder.h"

#include <QString>

#include "decoders/pidgin/PidginTextFormatDecoder.h"

#include "utils/fail.h"


RawConversationDecoder::RawConversationDecoder(RawConversationFile* convFile)
{
    convFile_ = convFile;
}

RawConversationDecoder::~RawConversationDecoder()
{
}

bool RawConversationDecoder::isSupportedFormat(RawConversationFile* convFile)
{
    return PidginTextFormatDecoder::recognize(convFile);
}

RawConversationDecoderUqPtr RawConversationDecoder::forRawConversationFile(
    RawConversationFile* convFile)
{
    if (PidginTextFormatDecoder::recognize(convFile)) {
        return RawConversationDecoderUqPtr(
            new PidginTextFormatDecoder(convFile));
    } else {
        fail("Conversation file '%s' is not in a supported format",
             qPrintable(convFile->description()));

        return RawConversationDecoderUqPtr(nullptr);
    }
}
