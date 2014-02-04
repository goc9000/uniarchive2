#include "RawStructuredSystemMessage.h"


bool RawStructuredSystemMessage::Param::isNull() const
{
    return (tag == RawStructuredSystemMessage::Param::EMPTY);
}

RawStructuredSystemMessage::Param::Type
RawStructuredSystemMessage::Param::type() const
{
    return tag;
}

QString RawStructuredSystemMessage::Param::description() const
{
    switch (tag) {
    case RawStructuredSystemMessage::Param::EMPTY:
        return "<null>";
    case RawStructuredSystemMessage::Param::SPEAKER:
        return speaker->description();
    }

    return "???";
}


QString RawStructuredSystemMessage::_sysMessageDescription() const
{
    QString desc;

    if (!subject.isNull()) {
        desc.append("S=[").append(subject.description()).append("]");
    }

    if (!desc.isEmpty()) {
        desc.append(", ");
    }
    desc.append("P=[")
        .append(SystemMessagePredicate_description(predicate))
        .append("]");

    if (!object.isNull()) {
        if (!desc.isEmpty()) {
            desc.append(", ");
        }
        desc.append("O=[").append(object.description()).append("]");
    }

    if (!aux.isNull()) {
        if (!desc.isEmpty()) {
            desc.append(", ");
        }
        desc.append("AUX=[").append(aux.description()).append("]");
    }

    return desc;
}
