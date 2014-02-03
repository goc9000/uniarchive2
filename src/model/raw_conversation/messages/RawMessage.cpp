#include "RawMessage.h"


QString RawMessage::description() const
{
    QString desc;

    desc.append("(").append(date.description()).append(") ");

    if (isOffline) {
        desc.append("[offline] ");
    }

    desc.append(_bodyDescription());

    return desc;
}
