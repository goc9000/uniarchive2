#include "PidginSystemMessageParser.h"

#include <QRegExp>

#include "model/raw_conversation/messages/RawOpaqueSystemMessage.h"
#include "model/raw_conversation/messages/RawStructuredSystemMessage.h"

#include "utils/fail.h"


class ParsingCase {
public:
    ParsingCase(QString pattern, QString subjectSpec,
                SystemMessagePredicate predicate, QString objectSpec="",
                QString auxSpec="")
        : regex(QRegExp(pattern)), subjectSpec(subjectSpec),
          predicate(predicate), objectSpec(objectSpec), auxSpec(auxSpec) {}

    RawStructuredSystemMessage::Param extractParam(
        QString spec, RawConversation& conversation);
    RawStructuredSystemMessage::Param parseParamFromCapture(
        QString captureText, QString typeName, RawConversation& conversation);

    QRegExp regex;
    QString subjectSpec;
    SystemMessagePredicate predicate;
    QString objectSpec;
    QString auxSpec;
};


RawStructuredSystemMessage::Param ParsingCase::extractParam(
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

RawStructuredSystemMessage::Param ParsingCase::parseParamFromCapture(
    QString captureText, QString typeName, RawConversation& conversation)
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


static std::vector<ParsingCase> PARSING_CASES = {
    ParsingCase(R"((.*) (?:has signed on|logged in)\.)",
                "1:speaker", SystemMessagePredicate::LOGGED_IN),
    ParsingCase(R"((.*) (?:has signed off|logged out)\.)",
                "1:speaker", SystemMessagePredicate::LOGGED_OUT),
    ParsingCase(R"((.*) has (?:gone|become) (away|idle)\.)",
                "1:speaker", SystemMessagePredicate::CHANGED_STATE, "2:state"),
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
                "1:speaker", SystemMessagePredicate::OFFERED_FILE, "2:file"),
    ParsingCase(R"(Offering to send (.*) to (.*))",
                "me", SystemMessagePredicate::OFFERED_FILE, "1:file",
                "2:speaker"),
    ParsingCase(R"((.*) is trying to send you a group of (\d+) files\.)",
                "1:speaker", SystemMessagePredicate::OFFERED_FILE_GROUP,
                "", "2:count"),
    ParsingCase(R"(Starting transfer of (.*) from (.*))",
                "", SystemMessagePredicate::FILE_TRANSFER_STARTED, "1:file",
                "2:speaker"),
    ParsingCase(R"((.*) canceled the transfer of (.*))",
                "1:speaker", SystemMessagePredicate::CANCELLED_FILE_TRANSFER,
                "2:file"),
    ParsingCase(R"(Transfer of file (.*) complete)",
                "", SystemMessagePredicate::FILE_TRANSFER_COMPLETE, "1:file"),
    ParsingCase(R"(Unable to send message: The message is too large\.)",
                "", SystemMessagePredicate::MESSAGE_SEND_FAILED, "",
                "send_err_msg_too_large"),
    ParsingCase(R"(Message could not be sent because the user is offline:)",
                "", SystemMessagePredicate::MESSAGE_SEND_FAILED, "",
                "send_err_peer_offline"),
    ParsingCase(R"(Message could not be sent because a connection )"
                R"(error occurred:)",
                "", SystemMessagePredicate::MESSAGE_SEND_FAILED,
                "", "send_err_connection"),
    ParsingCase(R"((.*) has sent you a webcam invite, which is not )"
                R"(yet supported\.)",
                "1:speaker",
                SystemMessagePredicate::SENT_WEBCAM_INVITE_UNSUPPORTED),
};


RawMessageUqPtr PidginSystemMessageParser::parseSystemMessage(
    TimeStamp messageDate, RawMessage::Flags flags, QString messageText,
    RawConversation& conversation)
{
    for (ParsingCase& parseCase : PARSING_CASES) {
        if (parseCase.regex.exactMatch(messageText)) {
            return RawMessageUqPtr(
                new RawStructuredSystemMessage(
                    messageDate,
                    flags,
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
        new RawOpaqueSystemMessage(messageDate, flags, messageText));
}
