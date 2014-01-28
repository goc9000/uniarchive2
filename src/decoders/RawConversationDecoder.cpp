#include "RawConversationDecoder.h"

#include <QString>

#include "utils/fail.h"


RawConversationDecoder::RawConversationDecoder(RawConversationFile* convFile)
{
    convFile_ = convFile;
}

RawConversationDecoder::~RawConversationDecoder()
{
}

bool RawConversationDecoder::isSupportedFormat(RawConversationFile *convFile)
{
    return false;
}

RawConversationDecoder* RawConversationDecoder::forRawConversationFile(
    RawConversationFile *convFile)
{
    fail("Conversation file '%s' is not in a supported format",
         qPrintable(convFile->description()));

    return nullptr;
}
