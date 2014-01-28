#ifndef FILESYSTEMRAWCONVERSATIONSOURCE_H
#define FILESYSTEMRAWCONVERSATIONSOURCE_H

#include <vector>

#include <QString>

#include "input/sources/RawConversationSource.h"


class FileSystemRawConversationSource : public RawConversationSource
{
public:
    FileSystemRawConversationSource(QString path);

    std::vector<RawConversation> rawConversations();

protected:
    QString path_;
};

#endif // FILESYSTEMRAWCONVERSATIONSOURCE_H
