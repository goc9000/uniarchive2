QT += core
QT += xml
QT -= gui

CONFIG += c++14

TARGET = uniarchive2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_LFLAGS_RELEASE -= -O1

SOURCES += \
    extraction/adium/extract_adium_conversations.cpp \
    extraction/digsby/extract_digsby_conversations.cpp \
    extraction/facebook/extract_facebook_dyi_conversations.cpp \
    extraction/msn/extract_msn_messenger_xml_conversations.cpp \
    extraction/pidgin/common_extract_pidgin_conversations_code.cpp \
    extraction/pidgin/extract_pidgin_html_conversations.cpp \
    extraction/pidgin/extract_pidgin_txt_conversations.cpp \
    extraction/skype/internal/query_skype_db.cpp \
    extraction/skype/extract_skype_conversations.cpp \
    extraction/whatsapp/extract_whatsapp_email_conversations.cpp \
    extraction/yahoo/ExtractYahooProtocolEventsIterator.cpp \
    extraction/yahoo/YahooProtocolEvent.cpp \
    extraction/yahoo/extract_yahoo_messenger_dat_conversations.cpp \
    extraction/extract_conversations_generic.cpp \
    extraction/parse_libpurple_system_message.cpp \
    graphics/ANSIColor.cpp \
    graphics/Color.cpp \
    intermediate_format/content/formatting/ANSIColorTag.cpp \
    intermediate_format/content/formatting/ANSIResetTag.cpp \
    intermediate_format/content/formatting/BoldTag.cpp \
    intermediate_format/content/formatting/CSSStyleTag.cpp \
    intermediate_format/content/formatting/EmphasisTag.cpp \
    intermediate_format/content/formatting/FontTag.cpp \
    intermediate_format/content/formatting/ItalicTag.cpp \
    intermediate_format/content/formatting/LinkTag.cpp \
    intermediate_format/content/formatting/RGBColorTag.cpp \
    intermediate_format/content/formatting/UnderlineTag.cpp \
    intermediate_format/content/formatting/YahooAltTag.cpp \
    intermediate_format/content/formatting/YahooFadeTag.cpp \
    intermediate_format/content/symbols/FlagIcon.cpp \
    intermediate_format/content/symbols/SkypeEmoticon.cpp \
    intermediate_format/content/AbstractTag.cpp \
    intermediate_format/content/LineBreak.cpp \
    intermediate_format/content/MediaAttachment.cpp \
    intermediate_format/content/RawMessageContent.cpp \
    intermediate_format/content/RawMessageContentItem.cpp \
    intermediate_format/content/RawMessageContentItemSubType.cpp \
    intermediate_format/content/SelfClosingTag.cpp \
    intermediate_format/content/SkypeQuote.cpp \
    intermediate_format/content/StandardTag.cpp \
    intermediate_format/content/SymmetricTag.cpp \
    intermediate_format/content/TextSection.cpp \
    intermediate_format/errors/CurrentCallFailReason.cpp \
    intermediate_format/errors/MediaError.cpp \
    intermediate_format/errors/OfferWebcamFailReason.cpp \
    intermediate_format/errors/PingFailReason.cpp \
    intermediate_format/errors/SendMessageFailReason.cpp \
    intermediate_format/errors/StartCallFailReason.cpp \
    intermediate_format/events/calls/RawAnswerCallEvent.cpp \
    intermediate_format/events/calls/RawEndCallEvent.cpp \
    intermediate_format/events/calls/RawOfferCallEvent.cpp \
    intermediate_format/events/calls/RawOfferWebcamEvent.cpp \
    intermediate_format/events/calls/RawStartCallEvent.cpp \
    intermediate_format/events/conference/RawAddToConferenceEvent.cpp \
    intermediate_format/events/conference/RawChangeConferencePictureEvent.cpp \
    intermediate_format/events/conference/RawChangeTopicEvent.cpp \
    intermediate_format/events/conference/RawCreateConferenceEvent.cpp \
    intermediate_format/events/conference/RawDeclineConferenceEvent.cpp \
    intermediate_format/events/conference/RawJoinConferenceEvent.cpp \
    intermediate_format/events/conference/RawLeaveConferenceEvent.cpp \
    intermediate_format/events/conference/RawRejoinConferenceEvent.cpp \
    intermediate_format/events/conference/RawRemoveFromConferenceEvent.cpp \
    intermediate_format/events/conference/RawSetSkypeChatRoleEvent.cpp \
    intermediate_format/events/conversation/RawJoinConversationEvent.cpp \
    intermediate_format/events/conversation/RawLeaveConversationEvent.cpp \
    intermediate_format/events/conversation/RawStartConversationEvent.cpp \
    intermediate_format/events/file_transfer/RawCancelFileTransferEvent.cpp \
    intermediate_format/events/file_transfer/RawOfferFileEvent.cpp \
    intermediate_format/events/file_transfer/RawOfferFileGroupEvent.cpp \
    intermediate_format/events/file_transfer/RawReceiveFileEvent.cpp \
    intermediate_format/events/file_transfer/RawStartFileTransferEvent.cpp \
    intermediate_format/events/file_transfer/RawTransferFilesEvent.cpp \
    intermediate_format/events/friending/RawContactDeleteEvent.cpp \
    intermediate_format/events/friending/RawContactRequestAcceptEvent.cpp \
    intermediate_format/events/friending/RawContactRequestEvent.cpp \
    intermediate_format/events/status/RawConnectedEvent.cpp \
    intermediate_format/events/status/RawDisconnectedEvent.cpp \
    intermediate_format/events/status/RawStatusChangeEvent.cpp \
    intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.cpp \
    intermediate_format/events/system/RawLoggingStartedEvent.cpp \
    intermediate_format/events/system/RawLoggingStoppedEvent.cpp \
    intermediate_format/events/system/RawWindowClosedEvent.cpp \
    intermediate_format/events/system/RawWindowOpenedEvent.cpp \
    intermediate_format/events/RawChangeScreenNameEvent.cpp \
    intermediate_format/events/RawCorruptedMessageEvent.cpp \
    intermediate_format/events/RawEditedPreviousMessageEvent.cpp \
    intermediate_format/events/RawEvent.cpp \
    intermediate_format/events/RawEventSubType.cpp \
    intermediate_format/events/RawMessageEvent.cpp \
    intermediate_format/events/RawPingEvent.cpp \
    intermediate_format/events/RawSendContactsEvent.cpp \
    intermediate_format/events/RawUninterpretedEvent.cpp \
    intermediate_format/provenance/AdiumArchiveFileProvenance.cpp \
    intermediate_format/provenance/ArchiveFileProvenance.cpp \
    intermediate_format/provenance/EventRangeProvenance.cpp \
    intermediate_format/provenance/MSNConversationProvenance.cpp \
    intermediate_format/provenance/Provenance.cpp \
    intermediate_format/provenance/ProvenanceSubType.cpp \
    intermediate_format/provenance/SkypeConversationProvenance.cpp \
    intermediate_format/subjects/AccountSubject.cpp \
    intermediate_format/subjects/ApparentSubject.cpp \
    intermediate_format/subjects/ApparentSubjectSubType.cpp \
    intermediate_format/subjects/FullySpecifiedSubject.cpp \
    intermediate_format/subjects/ImplicitSubject.cpp \
    intermediate_format/subjects/ImplicitSubjectKind.cpp \
    intermediate_format/subjects/ScreenNameSubject.cpp \
    intermediate_format/ApparentTime.cpp \
    intermediate_format/ApparentTimeReference.cpp \
    intermediate_format/RawConversation.cpp \
    intermediate_format/RawConversationCollection.cpp \
    intermediate_format/RawTransferredFile.cpp \
    protocols/digsby/digsby_account_name.cpp \
    protocols/facebook/facebook_account_name.cpp \
    protocols/jabber/jabber_account_name.cpp \
    protocols/msn/msn_account_name.cpp \
    protocols/phone/phone_number.cpp \
    protocols/skype/SkypeChatRole.cpp \
    protocols/skype/skype_account_name.cpp \
    protocols/yahoo/yahoo_account_name.cpp \
    protocols/ArchiveFormat.cpp \
    protocols/FullAccountName.cpp \
    protocols/IMProtocol.cpp \
    protocols/IMStatus.cpp \
    protocols/parse_account_generic.cpp \
    utils/external_libs/sqlite/sqlite3.c \
    utils/html/entities.cpp \
    utils/html/parse_html_lenient.cpp \
    utils/serialization/ISerializable.cpp \
    utils/sqlite/SQLiteDB.cpp \
    utils/sqlite/SQLiteRow.cpp \
    utils/sqlite/SQLiteStmt.cpp \
    utils/text/decoding.cpp \
    utils/text/load_text_file.cpp \
    utils/text/split_into_lines.cpp \
    utils/time/parse_date_parts.cpp \
    utils/xml/qdom_utils.cpp \
    main.cpp \
    temp_test_harness.cpp

HEADERS += \
    extraction/adium/extract_adium_conversations.h \
    extraction/digsby/extract_digsby_conversations.h \
    extraction/facebook/extract_facebook_dyi_conversations.h \
    extraction/msn/extract_msn_messenger_xml_conversations.h \
    extraction/pidgin/common_extract_pidgin_conversations_code.h \
    extraction/pidgin/extract_pidgin_html_conversations.h \
    extraction/pidgin/extract_pidgin_txt_conversations.h \
    extraction/skype/internal/RawSkypeCall.h \
    extraction/skype/internal/RawSkypeChat.h \
    extraction/skype/internal/RawSkypeConvo.h \
    extraction/skype/internal/RawSkypeIdentity.h \
    extraction/skype/internal/query_skype_db.h \
    extraction/skype/extract_skype_conversations.h \
    extraction/whatsapp/extract_whatsapp_email_conversations.h \
    extraction/yahoo/ExtractYahooProtocolEventsIterator.h \
    extraction/yahoo/YahooProtocolEvent.h \
    extraction/yahoo/extract_yahoo_messenger_dat_conversations.h \
    extraction/extract_conversations_generic.h \
    extraction/parse_libpurple_system_message.h \
    graphics/ANSIColor.h \
    graphics/Color.h \
    intermediate_format/content/formatting/ANSIColorTag.h \
    intermediate_format/content/formatting/ANSIResetTag.h \
    intermediate_format/content/formatting/BoldTag.h \
    intermediate_format/content/formatting/CSSStyleTag.h \
    intermediate_format/content/formatting/EmphasisTag.h \
    intermediate_format/content/formatting/FontTag.h \
    intermediate_format/content/formatting/ItalicTag.h \
    intermediate_format/content/formatting/LinkTag.h \
    intermediate_format/content/formatting/RGBColorTag.h \
    intermediate_format/content/formatting/UnderlineTag.h \
    intermediate_format/content/formatting/YahooAltTag.h \
    intermediate_format/content/formatting/YahooFadeTag.h \
    intermediate_format/content/symbols/FlagIcon.h \
    intermediate_format/content/symbols/SkypeEmoticon.h \
    intermediate_format/content/AbstractTag.h \
    intermediate_format/content/LineBreak.h \
    intermediate_format/content/MediaAttachment.h \
    intermediate_format/content/RawMessageContent.h \
    intermediate_format/content/RawMessageContentItem.h \
    intermediate_format/content/RawMessageContentItemSubType.h \
    intermediate_format/content/SelfClosingTag.h \
    intermediate_format/content/SkypeQuote.h \
    intermediate_format/content/StandardTag.h \
    intermediate_format/content/SymmetricTag.h \
    intermediate_format/content/TextSection.h \
    intermediate_format/errors/CurrentCallFailReason.h \
    intermediate_format/errors/MediaError.h \
    intermediate_format/errors/OfferWebcamFailReason.h \
    intermediate_format/errors/PingFailReason.h \
    intermediate_format/errors/SendMessageFailReason.h \
    intermediate_format/errors/StartCallFailReason.h \
    intermediate_format/events/calls/RawAnswerCallEvent.h \
    intermediate_format/events/calls/RawEndCallEvent.h \
    intermediate_format/events/calls/RawOfferCallEvent.h \
    intermediate_format/events/calls/RawOfferWebcamEvent.h \
    intermediate_format/events/calls/RawStartCallEvent.h \
    intermediate_format/events/conference/RawAddToConferenceEvent.h \
    intermediate_format/events/conference/RawChangeConferencePictureEvent.h \
    intermediate_format/events/conference/RawChangeTopicEvent.h \
    intermediate_format/events/conference/RawCreateConferenceEvent.h \
    intermediate_format/events/conference/RawDeclineConferenceEvent.h \
    intermediate_format/events/conference/RawJoinConferenceEvent.h \
    intermediate_format/events/conference/RawLeaveConferenceEvent.h \
    intermediate_format/events/conference/RawRejoinConferenceEvent.h \
    intermediate_format/events/conference/RawRemoveFromConferenceEvent.h \
    intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h \
    intermediate_format/events/conversation/RawJoinConversationEvent.h \
    intermediate_format/events/conversation/RawLeaveConversationEvent.h \
    intermediate_format/events/conversation/RawStartConversationEvent.h \
    intermediate_format/events/file_transfer/RawCancelFileTransferEvent.h \
    intermediate_format/events/file_transfer/RawOfferFileEvent.h \
    intermediate_format/events/file_transfer/RawOfferFileGroupEvent.h \
    intermediate_format/events/file_transfer/RawReceiveFileEvent.h \
    intermediate_format/events/file_transfer/RawStartFileTransferEvent.h \
    intermediate_format/events/file_transfer/RawTransferFilesEvent.h \
    intermediate_format/events/friending/RawContactDeleteEvent.h \
    intermediate_format/events/friending/RawContactRequestAcceptEvent.h \
    intermediate_format/events/friending/RawContactRequestEvent.h \
    intermediate_format/events/status/RawConnectedEvent.h \
    intermediate_format/events/status/RawDisconnectedEvent.h \
    intermediate_format/events/status/RawStatusChangeEvent.h \
    intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h \
    intermediate_format/events/system/RawLoggingStartedEvent.h \
    intermediate_format/events/system/RawLoggingStoppedEvent.h \
    intermediate_format/events/system/RawWindowClosedEvent.h \
    intermediate_format/events/system/RawWindowOpenedEvent.h \
    intermediate_format/events/RawChangeScreenNameEvent.h \
    intermediate_format/events/RawCorruptedMessageEvent.h \
    intermediate_format/events/RawEditedPreviousMessageEvent.h \
    intermediate_format/events/RawEvent.h \
    intermediate_format/events/RawEventSubType.h \
    intermediate_format/events/RawFailableEvent.h \
    intermediate_format/events/RawMessageEvent.h \
    intermediate_format/events/RawPingEvent.h \
    intermediate_format/events/RawSendContactsEvent.h \
    intermediate_format/events/RawUninterpretedEvent.h \
    intermediate_format/provenance/AdiumArchiveFileProvenance.h \
    intermediate_format/provenance/ArchiveFileProvenance.h \
    intermediate_format/provenance/EventRangeProvenance.h \
    intermediate_format/provenance/MSNConversationProvenance.h \
    intermediate_format/provenance/Provenance.h \
    intermediate_format/provenance/ProvenanceSubType.h \
    intermediate_format/provenance/SkypeConversationProvenance.h \
    intermediate_format/subjects/AccountSubject.h \
    intermediate_format/subjects/ApparentSubject.h \
    intermediate_format/subjects/ApparentSubjectSubType.h \
    intermediate_format/subjects/FullySpecifiedSubject.h \
    intermediate_format/subjects/ImplicitSubject.h \
    intermediate_format/subjects/ImplicitSubjectKind.h \
    intermediate_format/subjects/ScreenNameSubject.h \
    intermediate_format/ApparentTime.h \
    intermediate_format/ApparentTimeReference.h \
    intermediate_format/RawConversation.h \
    intermediate_format/RawConversationCollection.h \
    intermediate_format/RawTransferredFile.h \
    protocols/digsby/digsby_account_name.h \
    protocols/facebook/facebook_account_name.h \
    protocols/jabber/jabber_account_name.h \
    protocols/msn/msn_account_name.h \
    protocols/phone/phone_number.h \
    protocols/skype/SkypeChatRole.h \
    protocols/skype/skype_account_name.h \
    protocols/yahoo/yahoo_account_name.h \
    protocols/ArchiveFormat.h \
    protocols/FullAccountName.h \
    protocols/IMProtocol.h \
    protocols/IMStatus.h \
    protocols/parse_account_generic.h \
    utils/external_libs/sqlite/sqlite3.h \
    utils/external_libs/optional.hpp \
    utils/html/entities.h \
    utils/html/parse_html_lenient.h \
    utils/language/JavaStyleIterator.h \
    utils/language/callback_adapter.hpp \
    utils/language/invariant.h \
    utils/language/shortcuts.h \
    utils/language/template_utils.hpp \
    utils/polymorphics/IPolymorphic.h \
    utils/polymorphics/StandardPolymorphic.h \
    utils/qt/debug_extras.h \
    utils/qt/shortcuts.h \
    utils/serialization/IDeserializableDynamic.h \
    utils/serialization/IDeserializableStatic.h \
    utils/serialization/ISerializable.h \
    utils/serialization/deserialization_helpers.h \
    utils/serialization/serialization_helpers.h \
    utils/sqlite/internal/ColumnExtractor.hpp \
    utils/sqlite/internal/DataTupleGenerator.hpp \
    utils/sqlite/SQLiteDB.h \
    utils/sqlite/SQLiteRow.h \
    utils/sqlite/SQLiteStmt.h \
    utils/text/decoding.h \
    utils/text/load_text_file.h \
    utils/text/split_into_lines.h \
    utils/time/parse_date_parts.h \
    utils/xml/qdom_utils.h \
    temp_test_harness.h
