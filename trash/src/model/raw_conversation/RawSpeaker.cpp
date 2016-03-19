#include "RawSpeaker.h"


bool RawSpeaker::isNull() const
{
    return (alias.isNull() && !account && !isMeKnown);
}

QString RawSpeaker::description() const
{
    if (isNull()) {
        return "<speaker unknown>";
    }

    QString desc;

    if (!alias.isNull()) {
        desc.append(alias);
    }

    if (account) {
        if (!desc.isEmpty()) {
            desc.append(' ');
        }
        desc.append(QString("[") + account->description() + "]");
    }

    if (isMeKnown) {
        if (!desc.isEmpty()) {
            desc.append(' ');
        }
        desc.append(isMe ? "(me)" : "(not me)");
    }

    return desc;
}

void RawSpeaker::setNull()
{
    alias = QString::Null();
    account = nullptr;
    isMeKnown = false;
}
