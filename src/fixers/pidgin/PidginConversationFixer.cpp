#include "PidginConversationFixer.h"

#include "utils/fail.h"


RawConversation* PidginConversationFixer::fix()
{
    // TODO: fix 24-hour rollover and other date problems

    if (!conversation_->date.hasTimeZoneInfo()) {
        // TODO: try to fix absent timezone info using convFile_->
        // lastChangeDate() vs. last reply date
        warn("Timezone information absent");
    }

    return conversation_;
}
