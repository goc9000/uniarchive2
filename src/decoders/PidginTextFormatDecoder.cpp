#include "PidginTextFormatDecoder.h"

#include <QRegExp>
#include <QStringList>
#include <QDateTime>

#include "model/raw_conversation/RawAccount.h"
#include "model/raw_conversation/RawSpeaker.h"
#include "model/raw_conversation/messages/RawReply.h"
#include "model/raw_conversation/messages/RawOpaqueSystemMessage.h"
#include "model/raw_conversation/messages/RawStructuredSystemMessage.h"
#include "utils/fail.h"


#define MAX_ALIAS_LENGTH 32
#define MAX_ALIAS_SPACES 4


std::vector<RawConversation> PidginTextFormatDecoder::rawConversations()
{
    ErrorContext context("while decoding as Pidgin Text Format");

    RawConversation conversation = _getConversationBasics();

    if (conversation.isNull()) {
        warn("File is empty");
        return std::vector<RawConversation>();
    }

    _readMessages(conversation);

    _doLocalFixes(conversation);

    std::vector<RawConversation> result;
    result.push_back(std::move(conversation));

    return result;
}

bool PidginTextFormatDecoder::recognize(RawConversationFile *convFile)
{
    static QRegExp PAT_PIDGIN_FILE(
        R"(\d{4}-\d\d-\d\d\.\d{6}([+-]\d{4}\w*)?\.txt)");

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
        R"(Conversation with ([^ ]+) at (.+) on ([^ ]+) \((\w+)\))");

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
    outProtocol = IMProtocol_parseOrFail(PAT_HEADER_LINE.cap(4));

    return true;
}

void PidginTextFormatDecoder::_parseFriendId(
    QString idText, QString& outFriendId, bool& outIsConference) const
{
    static QRegExp PAT_CONFERENCE_ID(R"((.+)(-\d{6,}))");

    if (!PAT_CONFERENCE_ID.exactMatch(idText)) {
        outFriendId = idText;
        outIsConference = false;
        return;
    }

    outFriendId = PAT_CONFERENCE_ID.cap(1);
    outIsConference = true;
}

TimeStamp PidginTextFormatDecoder::_parseConversationDate(QString dateText)
    const
{
    static QRegExp PAT_CONV_DATE(
        R"((\d{4}-\d\d-\d\d)\.(\d{6})([+-]\d{4}\w*)?)");

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

void PidginTextFormatDecoder::_readMessages(RawConversation& conversation)
{
    QString dateText, messageText;
    int msgStartLine, msgEndLine;
    bool skipNextMessage = false;

    while (_readNextMessage(dateText, messageText, msgStartLine, msgEndLine)) {
        ErrorContext errContext(
            (msgEndLine == msgStartLine) ? "in message at line no. %d"
                                         : "in message at lines no. %d-%d",
            msgStartLine, msgEndLine);

        if (skipNextMessage) {
            skipNextMessage = false;
            continue;
        }

        RawMessageUqPtr message =
            _parseMessage(dateText, messageText, conversation);

        conversation.addMessage(message.release());

        if (messageText.startsWith("Message could not be sent")) {
            skipNextMessage = true;
        }
    }
}

bool PidginTextFormatDecoder::_readNextMessage(
    QString& outDateText, QString& outMessageText, int& outMsgStartLine,
    int& outMsgEndLine)
{
    ErrorContext errContext("at line no. %d", nextLineNo_);
    outMsgStartLine = outMsgEndLine = nextLineNo_;

    QString line = _readNextLine();
    if (line.isNull()) {
        return false;
    }

    if (!_isStartOfMessage(line)) {
        fail("Message format not recognized:\n'%s'", qPrintable(line));
    }

    int pos = line.indexOf(')');
    outDateText = line.mid(1, pos - 1);
    outMessageText = line.mid(pos + 1).trimmed();

    // Glue multi-line messages together
    while (true) {
        errContext.update("at line no. %d", nextLineNo_);
        line = _peekNextLine();
        if (line.isNull() || _isStartOfMessage(line)) {
            break;
        }

        line = _readNextLine();
        if (_resemblesStartOfMessage(line)) {
            warn("Line looks like a message, but date format is not "
                 "recognized:\n'%s'",
                 qPrintable(line));
        }

        outMessageText.append("\n").append(line);
        outMsgEndLine += 1;
    }

    return true;
}

bool PidginTextFormatDecoder::_isStartOfMessage(QString line) const
{
    static QRegExp PAT_MESSAGE(
        R"(\((\d\d/\d\d/\d{4} )?\d+:\d\d:\d\d\s*(am|pm)?\).*)",
        Qt::CaseInsensitive);

    return PAT_MESSAGE.exactMatch(line);
}

bool PidginTextFormatDecoder::_resemblesStartOfMessage(QString line) const
{
    static QRegExp PAT_MESSAGE_LIKE(R"(\((\d+[^)]*)\).*)");

    if (PAT_MESSAGE_LIKE.exactMatch(line)) {
        int numDigits = 0;
        for (QChar c : PAT_MESSAGE_LIKE.cap(1)) {
            if (c.isDigit()) {
                numDigits++;
            }
        }

        if ((numDigits >= 4) &&
            (numDigits < PAT_MESSAGE_LIKE.cap(1).length())) {
            return true;
        }
    }

    return false;
}

TimeStamp PidginTextFormatDecoder::_parseMessageDate(QString dateText) const
{
    static QRegExp PAT_MESSAGE_DATE(
        R"((\d\d/\d\d/\d{4} )?(\d+:\d\d:\d\d)\s*(am|pm)?)",
        Qt::CaseInsensitive);

    if (!PAT_MESSAGE_DATE.exactMatch(dateText)) {
        fail("Invalid message date format: '%s'", qPrintable(dateText));
    }

    QStringList parts = PAT_MESSAGE_DATE.capturedTexts();

    QDate datePart;
    if (!parts[1].isEmpty()) {
        datePart = QDate::fromString(parts[1].trimmed(), "MM/dd/yyyy");
        if (!datePart.isValid()) {
            fail("Invalid date specification: '%s'", qPrintable(dateText));
        }
    }

    QTime timePart = QTime::fromString(
        parts[2] + parts[3].toUpper(),
        parts[3].isEmpty() ? "hh:mm:ss" : "h:mm:ssAP");
    if (!timePart.isValid()) {
        fail("Invalid time specification: '%s'", qPrintable(dateText));
    }

    return TimeStamp(datePart, timePart);
}

void PidginTextFormatDecoder::_extractSpeakerName(
    QString messageText, QString& outSpeakerName,
    QString& outMessageContent) const
{
    int colonPos = messageText.indexOf(':');
    if ((colonPos == -1) ||
        messageText.startsWith("Message could not be sent") ||
        messageText.startsWith("Unable to send message"))
    {
        outSpeakerName = QString::Null();
        outMessageContent = messageText;
        return;
    }

    if (messageText == ": Buzz!!") {
        outSpeakerName = QString::Null();
        outMessageContent = "Buzz!!";
        return;
    }

    outSpeakerName = messageText.left(colonPos);
    outMessageContent = messageText.mid(colonPos + 1);

    if (_isAliasDubious(outSpeakerName)) {
        warn("Dubious alias: '%s'", qPrintable(outSpeakerName));
    }

    if (outMessageContent != "") {
        if (outMessageContent[0] == ' ') {
            outMessageContent.remove(0, 1);
        } else if (!outMessageContent[0].isSpace()) {
            fail("Sanity check failed while parsing speaker name, line is:\n"
                 "'%s'", qPrintable(messageText));
        }
    }
}

bool PidginTextFormatDecoder::_isAliasDubious(QString alias) const
{
    if (alias.length() > MAX_ALIAS_LENGTH) {
        return true;
    }

    int numSpaces = 0;
    for (QChar c : alias) {
        if (c.isSpace()) {
            numSpaces += 1;
        }
    }

    return (numSpaces > MAX_ALIAS_SPACES);
}

RawMessageUqPtr PidginTextFormatDecoder::_parseMessage(
    QString dateText, QString messageText, RawConversation& conversation) const
{
    TimeStamp messageDate = _parseMessageDate(dateText);
    messageDate.timeZone = conversation.date.timeZone;

    bool isOffline = !messageDate.date.isNull();

    QString speakerName;
    _extractSpeakerName(messageText, speakerName, messageText);

    if (speakerName.isNull()) {
        return _parseSystemMessage(messageDate, isOffline, messageText,
                                   conversation);
    }

    RawSpeaker* speaker = conversation.addSpeaker(speakerName);

    return RawMessageUqPtr(
        new RawReply(messageDate, isOffline, speaker, messageText));
}

RawMessageUqPtr PidginTextFormatDecoder::_parseSystemMessage(
    TimeStamp messageDate, bool isOffline, QString messageText,
    RawConversation& conversation) const
{
    struct ParsingCase {
        ParsingCase(QString pattern, QString subjectSpec,
                    SystemMessagePredicate predicate, QString objectSpec="",
                    QString auxSpec="")
            : regex(QRegExp(pattern)), subjectSpec(subjectSpec),
              predicate(predicate), objectSpec(objectSpec), auxSpec(auxSpec) {}

        RawStructuredSystemMessage::Param extractParam(
            QString spec, RawConversation& conversation)
        {
            static QRegExp PAT_FROM_CAPTURE_SPEC(R"((\d+):(\w+))");

            if (spec == "") {
                return RawStructuredSystemMessage::Param();
            } else if (spec == "me") {
                return RawStructuredSystemMessage::Param(
                    conversation.addSpeaker(true)
                );
            } else if (spec == "send_err_msg_too_large") {
                return RawStructuredSystemMessage::Param(
                    MessageSendFailedReason::MESSAGE_TOO_LARGE
                );
            } else if (spec == "send_err_peer_offline") {
                return RawStructuredSystemMessage::Param(
                    MessageSendFailedReason::PEER_IS_OFFLINE
                );
            }  else if (spec == "send_err_connection") {
                return RawStructuredSystemMessage::Param(
                    MessageSendFailedReason::CONNECTION_ERROR
                );
            } else if (PAT_FROM_CAPTURE_SPEC.exactMatch(spec)) {
                int capNo = PAT_FROM_CAPTURE_SPEC.cap(1).toInt();
                if ((capNo < 0) || (capNo > regex.captureCount())) {
                    fail("Invalid capture number '%d'", capNo);
                }

                return parseParamFromCapture(regex.cap(capNo),
                                             PAT_FROM_CAPTURE_SPEC.cap(2),
                                             conversation);
            } else {
                fail("Unsupported param spec: '%s'", qPrintable(spec));
            }

            return RawStructuredSystemMessage::Param();
        }

        RawStructuredSystemMessage::Param parseParamFromCapture(
            QString captureText, QString typeName,
            RawConversation& conversation)
        {
            if (typeName == "speaker") {
                return RawStructuredSystemMessage::Param(
                    conversation.addSpeaker(captureText)
                );
            } else if (typeName == "state") {
                return RawStructuredSystemMessage::Param(
                    PresenceState_parseOrFail(captureText)
                );
            } else if (typeName == "file") {
                return RawStructuredSystemMessage::Param(
                    conversation.addFile(captureText)
                );
            } else if (typeName == "count") {
                bool ok = false;
                unsigned int value = captureText.toUInt(&ok);
                if (!ok) {
                    fail("'%s' is not a valid count");
                }
                return RawStructuredSystemMessage::Param(value);
            } else {
                fail("Unsupported param conversion: '%s'",
                     qPrintable(typeName));
            }

            return RawStructuredSystemMessage::Param();
        }

        QRegExp regex;
        QString subjectSpec;
        SystemMessagePredicate predicate;
        QString objectSpec;
        QString auxSpec;
    };

    static std::vector<ParsingCase> PARSING_CASES = {
        ParsingCase(R"((.*) (?:has signed on|logged in)\.)",
                    "1:speaker", SystemMessagePredicate::LOGGED_IN),
        ParsingCase(R"((.*) (?:has signed off|logged out)\.)",
                    "1:speaker", SystemMessagePredicate::LOGGED_OUT),
        ParsingCase(R"((.*) has (?:gone|become) (away|idle)\.)",
                    "1:speaker", SystemMessagePredicate::CHANGED_STATE,
                    "2:state"),
        ParsingCase(R"((.*) is no longer (away|idle)\.)",
                    "1:speaker", SystemMessagePredicate::REVERTED_STATE),
        ParsingCase(R"((.*) is now known as (.*)\.)",
                    "1:speaker", SystemMessagePredicate::CHANGED_ALIAS,
                    "2:speaker"),
        ParsingCase(R"(Buzz!!)",
                    "me", SystemMessagePredicate::SENT_BUZZ),
        ParsingCase(R"((.*) just sent you a Buzz!)",
                    "1:speaker", SystemMessagePredicate::SENT_BUZZ),
        ParsingCase(R"((.*) entered the room\.)",
                    "1:speaker", SystemMessagePredicate::JOINED_CONFERENCE),
        ParsingCase(R"((.*) left the room\.)",
                    "1:speaker", SystemMessagePredicate::LEFT_CONFERENCE),
        ParsingCase(R"((.*) is offering to send file (.*))",
                    "1:speaker", SystemMessagePredicate::OFFERED_FILE,
                    "2:file"),
        ParsingCase(R"(Offering to send (.*) to (.*))",
                    "me", SystemMessagePredicate::OFFERED_FILE, "1:file",
                    "2:speaker"),
        ParsingCase(R"((.*) is trying to send you a group of (\d+) files\.)",
                    "1:speaker", SystemMessagePredicate::OFFERED_FILE_GROUP,
                    "", "2:count"),
        ParsingCase(R"(Starting transfer of (.*) from (.*))",
                    "", SystemMessagePredicate::FILE_TRANSFER_STARTED,
                    "1:file", "2:speaker"),
        ParsingCase(R"((.*) canceled the transfer of (.*))",
                    "1:speaker",
                    SystemMessagePredicate::CANCELLED_FILE_TRANSFER,
                    "2:file"),
        ParsingCase(R"(Transfer of file (.*) complete)",
                    "", SystemMessagePredicate::FILE_TRANSFER_COMPLETE,
                    "1:file"),
        ParsingCase(R"(Unable to send message: The message is too large\.)",
                    "", SystemMessagePredicate::MESSAGE_SEND_FAILED,
                    "", "send_err_msg_too_large"),
        ParsingCase(R"(Message could not be sent because the user is offline:)",
                    "", SystemMessagePredicate::MESSAGE_SEND_FAILED,
                    "", "send_err_peer_offline"),
        ParsingCase(R"(Message could not be sent because a connection )"
                    R"(error occurred:)",
                    "", SystemMessagePredicate::MESSAGE_SEND_FAILED,
                    "", "send_err_connection"),
        ParsingCase(R"((.*) has sent you a webcam invite, which is not )"
                    R"(yet supported\.)",
                    "1:speaker",
                    SystemMessagePredicate::SENT_WEBCAM_INVITE_UNSUPPORTED),
    };

    for (ParsingCase& parseCase : PARSING_CASES) {
        if (parseCase.regex.exactMatch(messageText)) {
            return RawMessageUqPtr(
                new RawStructuredSystemMessage(
                    messageDate,
                    isOffline,
                    parseCase.extractParam(parseCase.subjectSpec, conversation),
                    parseCase.predicate,
                    parseCase.extractParam(parseCase.objectSpec, conversation),
                    parseCase.extractParam(parseCase.auxSpec, conversation)
                )
            );
        }
    }

    warn("Unrecognized system message format, storing as-is:\n\"%s\"",
         qPrintable(messageText));

    return RawMessageUqPtr(
        new RawOpaqueSystemMessage(messageDate, isOffline, messageText));
}

void PidginTextFormatDecoder::_doLocalFixes(RawConversation &conversation)
{
    // TODO: fix 24-hour rollover and other date problems

    if (!conversation.date.hasTimeZoneInfo()) {
        // TODO: try to fix absent timezone info using convFile_->
        // lastChangeDate() vs. last reply date
        warn("Timezone information absent");
    }
}
