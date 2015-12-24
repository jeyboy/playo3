#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T03:22:26
#
#-------------------------------------------------

QT       += core testlib gui

#QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    test_filename_conversions.cpp \
    test_html_parser.cpp \

HEADERS += \
    test_filename_conversions.h \
    test_html_parser.h \
