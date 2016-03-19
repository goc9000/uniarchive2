#include "RawConversation.h"

#include "utils/utils.h"

#include <iostream>


RawConversation::RawConversation(RawConversation&& other)
    : date(other.date), kind(other.kind), myAccount(std::move(other.myAccount))
{
    appendByMoving(friendAccounts, other.friendAccounts);
    appendByMoving(speakers, other.speakers);
    appendByMoving(files, other.files);
    appendByMoving(messages, other.messages);
}

bool RawConversation::isNull() const
{
    return date.isNull() && !myAccount;
}

void RawConversation::dump(bool showMessages) const
{
    if (isNull())  {
        std::cout << "<null conversation>" << std::endl;
        return;
    }

    std::cout << qPrintable(ConversationKind_description(kind));
    std::cout << " on ";
    std::cout << qPrintable(myAccount->description());
    std::cout << " at ";
    std::cout << qPrintable(date.description());
    std::cout << " with ";

    bool first = true;
    for (const RawAccountUqPtr& account : friendAccounts) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << qPrintable(account->description());

        first = false;
    }

    std::cout << std::endl;

    if (showMessages) {
        for (const RawMessageUqPtr& message : messages) {
            std::cout << "    " << qPrintable(message->description())
                      << std::endl;
        }
    }
}

RawAccount* RawConversation::getAccount(QString id, IMProtocol protocol) const
{
    if ((myAccount->id == id) && (myAccount->protocol == protocol)) {
        return myAccount.get();
    }

    for (const RawAccountUqPtr& account : friendAccounts) {
        if ((account->id == id) && (account->protocol == protocol)) {
            return account.get();
        }
    }

    return nullptr;
}

RawAccount* RawConversation::addFriendAccount(QString id, IMProtocol protocol)
{
    RawAccount* account = getAccount(id, protocol);
    if (account) {
        return account;
    }

    account = new RawAccount(id, protocol);
    friendAccounts.emplace_back(RawAccountUqPtr(account));

    return account;
}

RawSpeaker* RawConversation::getSpeaker(
    QString alias, RawAccount* account, bool isMeKnown, bool isMe) const
{
    for (const RawSpeakerUqPtr& speaker : speakers) {
        if ((speaker->alias == alias) &&
            (speaker->account == account) &&
            (speaker->isMeKnown == isMeKnown) &&
            (!isMeKnown || (speaker->isMe == isMe))) {
            return speaker.get();
        }
    }

    return nullptr;
}

RawSpeaker* RawConversation::getSpeaker(
    RawAccount* account, bool isMeKnown, bool isMe) const
{
    return getSpeaker(QString::Null(), account, isMeKnown, isMe);
}

RawSpeaker* RawConversation::getSpeaker(bool isMe) const
{
    return getSpeaker(QString::Null(), nullptr, true, isMe);
}

RawSpeaker* RawConversation::addSpeaker(
    QString alias, RawAccount* account, bool isMeKnown, bool isMe)
{
    RawSpeaker* speaker = getSpeaker(alias, account, isMeKnown, isMe);
    if (speaker) {
        return speaker;
    }

    speaker = new RawSpeaker(alias, account, isMeKnown, isMe);
    speakers.emplace_back(RawSpeakerUqPtr(speaker));

    return speaker;
}

RawSpeaker* RawConversation::addSpeaker(
    RawAccount* account, bool isMeKnown, bool isMe)
{
    return addSpeaker(QString::Null(), account, isMeKnown, isMe);
}

RawSpeaker* RawConversation::addSpeaker(bool isMe)
{
    return addSpeaker(QString::Null(), nullptr, true, isMe);
}

RawTransferredFileRef* RawConversation::getFile(QString filename) const
{
    for (const RawTransferredFileRefUqPtr& file : files) {
        if (file->filename == filename) {
            return file.get();
        }
    }

    return nullptr;
}

RawTransferredFileRef* RawConversation::addFile(QString filename)
{
    RawTransferredFileRef* file = getFile(filename);
    if (file) {
        return file;
    }

    file = new RawTransferredFileRef(filename);
    files.emplace_back(RawTransferredFileRefUqPtr(file));

    return file;
}

void RawConversation::addMessage(RawMessage* message)
{
    messages.emplace_back(RawMessageUqPtr(message));
}
