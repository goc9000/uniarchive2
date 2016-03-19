#include "RawConversationSource.h"

#include "utils/fail.h"
#include "input/sources/FileSystemRawConversationSource.h"


RawConversationSource::RawConversationSource()
{
}

RawConversationSource::~RawConversationSource()
{
}

RawConversationSourceUqPtr
RawConversationSource::fromSpecification(QString spec)
{
    return
        RawConversationSourceUqPtr(new FileSystemRawConversationSource(spec));

    fail("'%s' is not recognized as a valid source of conversations",
         qPrintable(spec));

    return RawConversationSourceUqPtr(nullptr);
}
