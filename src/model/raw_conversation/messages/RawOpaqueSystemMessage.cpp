#include "RawOpaqueSystemMessage.h"


QString RawOpaqueSystemMessage::_sysMessageDescription() const
{
    return QString("\"").append(content).append("\"");
}
