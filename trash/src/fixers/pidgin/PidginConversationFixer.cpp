#include "PidginConversationFixer.h"

#include <QHash>

#include "debug/conversations.h"
#include "utils/fail.h"


RawConversation* PidginConversationFixer::fix()
{
    //_detectQuotedMessages();

    find_temporal_discontinuities(conversation_);


    // TODO: fix 24-hour rollover and other date problems

    if (!conversation_->date.hasTimeZoneInfo()) {
        // TODO: try to fix absent timezone info using convFile_->
        // lastChangeDate() vs. last reply date
        //warn("Timezone information absent");
    }

    return conversation_;
}

void PidginConversationFixer::_detectQuotedMessages()
{
    QHash<QString, RawMessage*> messagesHash;

    for (RawMessageUqPtr& message : conversation_->messages) {
        QString key = message->description();

        if (messagesHash.contains(key)) {
            warn("Found!");
        } else {
            messagesHash[key] = message.get();
        }
        /*
        if (message->isOffline()) continue;

        int delta = message->date.time.secsTo(conversation.date.time);
        if (delta > 60*20) {
            conversation.dump();
            //warn("PAF: %s : %s", qPrintable(conversation.date.description()), qPrintable(message->description()));
        }
        break;*/
    }
}


// IDEA: message holds "date seen" (deffo sorted) and "date sent" (may be
// discontinuous)


// IDEA: could not send message, should point to message that could not be
// sent, just like quoted messages point to quotee

/*
for (RawMessageUqPtr& message : conversation.messages) {
    if (message->isOffline()) continue;

    int delta = message->date.time.secsTo(conversation.date.time);
    if (delta > 60*20) {
        conversation.dump();
        //warn("PAF: %s : %s", qPrintable(conversation.date.description()), qPrintable(message->description()));
    }
    break;
}
*/
