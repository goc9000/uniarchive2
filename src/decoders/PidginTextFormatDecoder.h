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
    RawConversation _getConversationBasics();
    bool _readHeader(QString& outMyId, QString& outFriendId,
                     IMProtocol &outProtocol, bool& outIsConference);
    void _parseFriendId(QString idText, QString& outFriendId,
                        bool& outIsConference) const;
    IMProtocol _parseProtocol(QString protoText) const;
    TimeStamp _parseConversationDate(QString dateText) const;
};

#endif // PIDGINTEXTFORMATDECODER_H