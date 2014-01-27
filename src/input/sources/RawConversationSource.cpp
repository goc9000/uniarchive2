#include "RawConversationSource.h"

#include "utils/fail.h"


RawConversationSource* RawConversationSource::fromSpecification(QString spec)
{
    fail("'%s' is not recognized as a valid source of conversations",
         qPrintable(spec));

    return nullptr;
}
