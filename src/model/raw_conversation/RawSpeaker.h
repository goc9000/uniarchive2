#ifndef RAWSPEAKER_H
#define RAWSPEAKER_H

#include <QString>

#include <memory>

#include "model/raw_conversation/RawAccount.h"


class RawSpeaker;
typedef std::unique_ptr<RawSpeaker> RawSpeakerUqPtr;

class RawSpeaker
{
public:
    RawSpeaker()
        : alias(QString::Null()), account(nullptr), isMeKnown(false) {}
    RawSpeaker(QString alias, RawAccount* account=nullptr,
               bool isMeKnown=false, bool isMe=true)
        : alias(alias), account(account), isMeKnown(isMeKnown), isMe(isMe) {}
    RawSpeaker(RawAccount* account, bool isMeKnown=false, bool isMe=true)
        : alias(QString::Null()), account(account), isMeKnown(isMeKnown),
          isMe(isMe) {}
    RawSpeaker(bool isMe)
        : alias(QString::Null()), account(nullptr), isMeKnown(true),
          isMe(isMe) {}

    QString alias;
    RawAccount* account;
    bool isMeKnown;
    bool isMe;

    bool isNull() const;

    QString description() const;

    void setNull();
};


#endif // RAWSPEAKER_H
