#include "RawReply.h"


QString RawReply::_bodyDescription() const
{
    QString desc;

    if (!speaker) {
        desc.append("<speaker unknown>");
    } else {
        desc.append(speaker->description());
    }

    desc.append(": \"").append(content).append("\"");

    return desc;
}
