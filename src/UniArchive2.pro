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
    utils/utils.cpp

HEADERS += \
    core/args.h \
    core/about.h \
    utils/fail.h \
    utils/utils.h
