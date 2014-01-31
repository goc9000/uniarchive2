#include "PidginTextFormatDecoder.h"

#include <QRegExp>
#include <QStringList>

#include "utils/fail.h"


std::vector<RawConversation> PidginTextFormatDecoder::rawConversations()
{
    ErrorContext context("while decoding as Pidgin Text Format");

    _startReading();

    QString myId, friendId;
    IMProtocol protocol;
    bool isConference;

    std::vector<RawConversation> result;
    if (!_readHeader(myId, friendId, protocol, isConference)) {
        warn("File is empty");
        return result;
    }

    return result;
}

bool PidginTextFormatDecoder::recognize(RawConversationFile *convFile)
{
    static QRegExp PAT_PIDGIN_FILE(
        R"(^\d{4}-\d{2}-\d{2}\.\d{6}([+-]\d{4}\w*)?\.txt$)");

    return PAT_PIDGIN_FILE.exactMatch(convFile->filename());
}

bool PidginTextFormatDecoder::_readHeader(QString& outMyId,
        QString& outFriendId, IMProtocol &outProtocol, bool& outIsConference)
{
    static QRegExp PAT_HEADER_LINE(
        R"(^Conversation with ([^ ]+) at (.+) on ([^ ]+) \((\w+)\)$)");

    ErrorContext context("on first line");

    QString line = _readNextLine();
    if (line.isNull() || (line.trimmed() == "")) {
        return false;
    }

    if (!PAT_HEADER_LINE.exactMatch(line)) {
        fail("Header does not match expected format:\n\"%s\"",
             qPrintable(line));
    }

    outMyId = PAT_HEADER_LINE.cap(3);
    _parseFriendId(PAT_HEADER_LINE.cap(1), outFriendId, outIsConference);
    outProtocol = _parseProtocol(PAT_HEADER_LINE.cap(4));

    return true;
}

void PidginTextFormatDecoder::_parseFriendId(QString idText,
        QString& outFriendId, bool& outIsConference)
{
    static QRegExp PAT_CONFERENCE_ID(R"(^(.+)(-\d{6,})$)");

    if (!PAT_CONFERENCE_ID.exactMatch(idText)) {
        outFriendId = idText;
        outIsConference = false;
        return;
    }

    outFriendId = PAT_CONFERENCE_ID.cap(1);
    outIsConference = true;
}

IMProtocol PidginTextFormatDecoder::_parseProtocol(QString protoText)
{
    if (protoText == "yahoo") {
        return IMProtocol::YAHOO;
    } else if (protoText == "jabber") {
        return IMProtocol::XMPP;
    } else if (protoText == "msn") {
        return IMProtocol::MSN;
    }

    fail("Unrecognized protocol string '%s'", qPrintable(protoText));

    return IMProtocol::INVALID;
}

