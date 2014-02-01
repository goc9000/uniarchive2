#ifndef PIDGINTEXTFORMATDECODER_H
#define PIDGINTEXTFORMATDECODER_H

#include "decoders/TextBasedDecoder.h"
#include "model/constants.h"
#include "model/time/TimeStamp.h"


class PidginTextFormatDecoder : public TextBasedDecoder
{
public:
    using TextBasedDecoder::TextBasedDecoder;

    std::vector<RawConversation> rawConversations();

    static bool recognize(RawConversationFile *convFile);
private:
    bool _readHeader(QString& outMyId, QString& outFriendId,
                     IMProtocol &outProtocol, bool& outIsConference);
    void _parseFriendId(QString idText, QString& outFriendId,
                        bool& outIsConference);
    IMProtocol _parseProtocol(QString protoText);
    TimeStamp _parseConversationDate(QString dateText);
};

#endif // PIDGINTEXTFORMATDECODER_H
