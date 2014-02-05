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
    TimeStamp _parseConversationDate(QString dateText) const;

    void _readMessages(RawConversation& conversation);
    bool _readNextMessage(QString& outDateText, QString& outMessageText,
                          int& outMsgStartLine, int& outMsgEndLine);
    bool _isStartOfMessage(QString line) const;
    bool _resemblesStartOfMessage(QString line) const;
    TimeStamp _parseMessageDate(QString dateText) const;
    void _extractSpeakerName(QString messageText, QString& outSpeakerName,
                             QString& outMessageContent) const;
    bool _isAliasDubious(QString alias) const;
    RawMessageUqPtr _parseMessage(QString dateText, QString messageText,
                                  RawConversation& conversation) const;
    RawMessageUqPtr _parseSystemMessage(TimeStamp messageDate, bool isOffline,
                                        QString messageText,
                                        RawConversation& conversation) const;
};

#endif // PIDGINTEXTFORMATDECODER_H
