#-------------------------------------------------
#
# Project created by QtCreator 2015-06-09T22:24:11
#
#-------------------------------------------------

QT       += network webkit webkitwidgets

QT       -= gui

TARGET = web_plugin
TEMPLATE = lib
CONFIG += release

DEFINES += WEB_PLUGIN_LIBRARY

SOURCES += webplugin.cpp \
    web_dialog.cpp

HEADERS += webplugin.h\
        web_plugin_global.h \
    web_dialog_interface.h \
    web_dialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    web_dialog.ui
