#ifndef RAWCONVERSATION_H
#define RAWCONVERSATION_H

#include <vector>

#include "model/time/TimeStamp.h"
#include "model/raw_conversation/RawAccount.h"
#include "model/raw_conversation/RawSpeaker.h"
#include "model/raw_conversation/RawTransferredFileRef.h"
#include "model/raw_conversation/messages/RawMessage.h"
#include "model/constants.h"


class RawConversation
{
public:
    RawConversation()
        : date(), kind(ConversationKind::INVALID), myAccount(nullptr) {}
    RawConversation(TimeStamp date, ConversationKind kind, RawAccount myAccount)
        : date(date), kind(kind), myAccount(new RawAccount(myAccount)) {}
    RawConversation(RawConversation&& other);

    TimeStamp date;
    ConversationKind kind;

    RawAccountUqPtr myAccount;
    std::vector<RawAccountUqPtr> friendAccounts;

    std::vector<RawSpeakerUqPtr> speakers;
    std::vector<RawTransferredFileRefUqPtr> files;

    std::vector<RawMessageUqPtr> messages;

    bool isNull() const;

    void dump(bool showMessages=true) const;

    RawAccount* getAccount(QString id, IMProtocol protocol) const;
    RawAccount* addFriendAccount(QString id, IMProtocol protocol);

    RawSpeaker* getSpeaker(QString alias, RawAccount* account=nullptr,
                           bool isMeKnown=false, bool isMe=true) const;
    RawSpeaker* getSpeaker(RawAccount* account, bool isMeKnown=false,
                           bool isMe=true) const;
    RawSpeaker* getSpeaker(bool isMe) const;

    RawSpeaker* addSpeaker(QString alias, RawAccount* account=nullptr,
                           bool isMeKnown=false, bool isMe=true);
    RawSpeaker* addSpeaker(RawAccount* account, bool isMeKnown=false,
                           bool isMe=true);
    RawSpeaker* addSpeaker(bool isMe);

    RawTransferredFileRef* getFile(QString filename) const;
    RawTransferredFileRef* addFile(QString filename);

    void addMessage(RawMessage* message);
};

#endif // RAWCONVERSATION_H
