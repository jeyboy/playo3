#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T00:45:08
#
#-------------------------------------------------

VERSION = 0.6.0.0

CONFIG += c++11
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
#DEFINES *= QT_USE_QSTRINGBUILDER

QT  += core gui network
#QT       += multimedia multimediawidgets opengl

win32: {
    QT  += winextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#SUBDIRS += tests # uncomment for tests running

TARGET = playo3
TEMPLATE = app

##profiling
#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg
###############

#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O3

#QMAKE_LFLAGS_RELEASE -= -O1

#QMAKE_CXXFLAGS+= -fopenmp
#QMAKE_LFLAGS +=  -fopenmp

FORMS    += playo.ui \
    dialogs/settings_dialog.ui \
    dialogs/tabdialog.ui \
    dialogs/extension_dialog.ui \
    dialogs/relations_dialog.ui \
    dialogs/search_dialog.ui \
    dialogs/echonest_dialog.ui \
    dialogs/user_action_dialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = playo.rc

unix:!mac {
        QT += gui-private
        CONFIG += link_pkgconfig release
        PKGCONFIG += x11
        LDFLAGS =- lX11
        DEFINES += HAVE_X11
        SOURCES += globalshortcut/qxtglobalshortcut_x11.cpp
}
win32: { SOURCES += globalshortcut/qxtglobalshortcut_win.cpp }
mac: {   SOURCES += globalshortcut/qxtglobalshortcut_mac.cpp }

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/taglib/ -ltaglib-project
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/taglib/ -ltaglib-project
#else:unix:!macx: LIBS += -L$$PWD/libs/taglib/ -ltaglib-project

LIBS += -L$$PWD/libs/taglib/ -ltaglib-project
INCLUDEPATH += $$PWD/libs/taglib/include
DEPENDPATH += $$PWD/libs/taglib

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/bass/ -lbass -lbass_fx -lbassmix
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/bass/ -lbass -lbass_fx -lbassmix
#else:unix:!macx: LIBS += -L$$PWD/libs/bass/ -lbass -lbass_fx -lbassmix

# add wasapi plugin
LIBS += -L$$PWD/libs/bass/ -lbass -lbass_fx -lbassmix
INCLUDEPATH += $$PWD/libs/bass/include
DEPENDPATH += $$PWD/libs/bass
