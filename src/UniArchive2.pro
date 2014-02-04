#-------------------------------------------------
#
# Project created by QtCreator 2014-01-25T23:18:50
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

TARGET = uniarchive2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ../bin

SOURCES += main.cpp \
    core/args.cpp \
    core/about.cpp \
    utils/fail.cpp \
    utils/utils.cpp \
    input/sources/RawConversationSource.cpp \
    model/raw_conversation/RawConversation.cpp \
    input/sources/FileSystemRawConversationSource.cpp \
    input/file_scanners/FileSystemScanner.cpp \
    input/file_scanners/RawConversationFile.cpp \
    input/file_scanners/RegularRawConversationFile.cpp \
    decoders/RawConversationDecoder.cpp \
    decoders/PidginTextFormatDecoder.cpp \
    utils/qt_utils.cpp \
    decoders/TextBasedDecoder.cpp \
    model/constants.cpp \
    model/time/TimeStamp.cpp \
    model/time/TimeZoneInfo.cpp \
    model/raw_conversation/RawAccount.cpp \
    model/raw_conversation/RawSpeaker.cpp \
    model/raw_conversation/messages/RawMessage.cpp \
    model/raw_conversation/messages/RawReply.cpp \
    model/raw_conversation/messages/RawSystemMessage.cpp \
    model/raw_conversation/messages/RawOpaqueSystemMessage.cpp \
    model/raw_conversation/messages/RawStructuredSystemMessage.cpp

HEADERS += \
    core/args.h \
    core/about.h \
    utils/fail.h \
    utils/utils.h \
    input/sources/RawConversationSource.h \
    model/raw_conversation/RawConversation.h \
    input/sources/FileSystemRawConversationSource.h \
    input/file_scanners/FileSystemScanner.h \
    input/file_scanners/RawConversationFile.h \
    input/file_scanners/RegularRawConversationFile.h \
    decoders/RawConversationDecoder.h \
    decoders/PidginTextFormatDecoder.h \
    utils/qt_utils.h \
    decoders/TextBasedDecoder.h \
    model/constants.h \
    model/time/TimeStamp.h \
    model/time/TimeZoneInfo.h \
    model/raw_conversation/RawAccount.h \
    model/raw_conversation/RawSpeaker.h \
    model/raw_conversation/messages/RawMessage.h \
    model/raw_conversation/messages/RawReply.h \
    model/raw_conversation/messages/RawSystemMessage.h \
    model/raw_conversation/messages/RawOpaqueSystemMessage.h \
    model/raw_conversation/messages/RawStructuredSystemMessage.h
