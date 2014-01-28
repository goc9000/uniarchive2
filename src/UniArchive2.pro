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
    decoders/RawConversationDecoder.cpp

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
    decoders/RawConversationDecoder.h
