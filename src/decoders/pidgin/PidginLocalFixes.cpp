#include "PidginLocalFixes.h"

#include "utils/fail.h"


void PidginLocalFixes::fixConversation(RawConversation &conversation,
                                       RawConversationFile* sourceFile)
{
    // TODO: fix 24-hour rollover and other date problems

    if (!conversation.date.hasTimeZoneInfo()) {
        // TODO: try to fix absent timezone info using convFile_->
        // lastChangeDate() vs. last reply date
        warn("Timezone information absent");
    }
}
