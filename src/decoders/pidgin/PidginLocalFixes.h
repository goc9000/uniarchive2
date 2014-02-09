#ifndef PIDGINLOCALFIXES_H
#define PIDGINLOCALFIXES_H

#include "model/raw_conversation/RawConversation.h"
#include "input/file_scanners/RawConversationFile.h"


class PidginLocalFixes
{
public:
    static void fixConversation(RawConversation &conversation,
                                RawConversationFile* sourceFile);
};

#endif // PIDGINLOCALFIXES_H
