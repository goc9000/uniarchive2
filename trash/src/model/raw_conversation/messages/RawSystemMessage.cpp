#include "RawSystemMessage.h"


QString RawSystemMessage::_bodyDescription() const
{
    return QString("SYSTEM: ").append(_sysMessageDescription());
}
