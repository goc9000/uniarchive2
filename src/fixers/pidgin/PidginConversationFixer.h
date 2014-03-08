#ifndef PIDGINCONVERSATIONFIXER_H
#define PIDGINCONVERSATIONFIXER_H

#include "input/file_scanners/RawConversationFile.h"
#include "model/raw_conversation/RawConversation.h"


class PidginConversationFixer
{
public:
    PidginConversationFixer(RawConversation* conversation,
                            RawConversationFile* sourceFile)
        : conversation_(conversation), sourceFile_(sourceFile) {}

    RawConversation* fix();

protected:
    RawConversation* conversation_;
    RawConversationFile* sourceFile_;
};

#endif // PIDGINCONVERSATIONFIXER_H
