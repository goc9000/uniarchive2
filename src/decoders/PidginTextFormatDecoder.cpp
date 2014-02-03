#include "PidginTextFormatDecoder.h"

#include <QRegExp>
#include <QStringList>
#include <QDateTime>

#include "model/raw_conversation/RawAccount.h"
#include "utils/fail.h"


std::vector<RawConversation> PidginTextFormatDecoder::rawConversations()
{
    ErrorContext context("while decoding as Pidgin Text Format");

    RawConversation conversation = _getConversationBasics();

    if (conversation.isNull()) {
        warn("File is empty");
        return std::vector<RawConversation>();
    }

    // TODO: read replies

    if (!conversation.date.hasTimeZoneInfo()) {
        // TODO: try to fix absent timezone info using convFile_->
        // lastChangeDate() vs. last reply date
        warn("Timezone information absent");
    }

    std::vector<RawConversation> result;
    result.push_back(std::move(conversation));

    return result;
}

bool PidginTextFormatDecoder::recognize(RawConversationFile *convFile)
{
    static QRegExp PAT_PIDGIN_FILE(
        R"(^\d{4}-\d\d-\d\d\.\d{6}([+-]\d{4}\w*)?\.txt$)");

    return PAT_PIDGIN_FILE.exactMatch(convFile->filename());
}

RawConversation PidginTextFormatDecoder::_getConversationBasics()
{
    _startReading();

    QString myId, friendId;
    IMProtocol protocol;
    bool isConference;

    if (!_readHeader(myId, friendId, protocol, isConference)) {
        return RawConversation();
    }

    RawConversation conversation(_parseConversationDate(convFile_->basename()),
                                 isConference ? ConversationKind::CONFERENCE
                                              : ConversationKind::ONE_ON_ONE,
                                 RawAccount(myId, protocol));

    conversation.addFriendAccount(friendId, protocol);

    return conversation;
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
    if (outMyId.endsWith('/')) {
        outMyId.chop(1);
    }

    _parseFriendId(PAT_HEADER_LINE.cap(1), outFriendId, outIsConference);
    outProtocol = _parseProtocol(PAT_HEADER_LINE.cap(4));

    return true;
}

void PidginTextFormatDecoder::_parseFriendId(
    QString idText, QString& outFriendId, bool& outIsConference) const
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

IMProtocol PidginTextFormatDecoder::_parseProtocol(QString protoText) const
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

TimeStamp PidginTextFormatDecoder::_parseConversationDate(QString dateText)
    const
{
    static QRegExp PAT_CONV_DATE(
        R"(^(\d{4}-\d\d-\d\d)\.(\d{6})([+-]\d{4}\w*)?$)");

    if (!PAT_CONV_DATE.exactMatch(dateText)) {
        fail("Invalid conversation date format: '%s'", qPrintable(dateText));
    }

    QStringList parts = PAT_CONV_DATE.capturedTexts();

    QDate date = QDate::fromString(parts[1], "yyyy-MM-dd");
    if (!date.isValid()) {
        fail("Invalid date specification: '%s'", qPrintable(parts[1]));
    }
    QTime time = QTime::fromString(parts[2], "hhmmss");
    if (!time.isValid()) {
        fail("Invalid time specification: '%s", qPrintable(parts[2]));
    }
    TimeZoneInfo timeZone = TimeZoneInfo::fromString(parts[3]);

    return TimeStamp(date, time, timeZone);
}
