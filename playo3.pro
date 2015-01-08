#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T00:45:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = playo3
TEMPLATE = app


SOURCES += main.cpp\
        playo.cpp \
    modules/json/data_store.cpp \
    modules/controls/tabber.cpp \
    modules/controls/tab.cpp \
    dockbars.cpp \
    modules/models/_base_model.cpp \
    modules/model_items/items/_base_item.cpp \
    modules/model_items/items/item_interface.cpp \
    modules/models/model_interface.cpp \
    modules/model_items/item_state.cpp \
    toolbars.cpp \
    modules/controls/toolbar.cpp \
    modules/controls/toolbarbutton.cpp \
    dialogs/toolbarbuttondialog.cpp \
    dialogs/toolbardialog.cpp \
    single_application.cpp \
    modules/controls/main_window.cpp \
    modules/controls/button.cpp \
    modules/controls/clickable_label.cpp \
    modules/controls/hoverable_label.cpp \
    dialogs/settings_dialog.cpp \
    dialogs/tabdialog.cpp \
    misc/settings.cpp \
    misc/hotkey_manager.cpp \
    media/audio_player.cpp \
    media/player.cpp \
    media/duration.cpp \
    media/format.cpp \
    media/notify_timer.cpp \
    modules/models/hotkey_model/hotkey_delegate.cpp \
    modules/models/hotkey_model/hotkey_editor.cpp \
    modules/models/hotkey_model/hotkey_model.cpp \
    modules/models/hotkey_model/hotkey_model_item.cpp \
    globalshortcut/qxtglobalshortcut.cpp \
    modules/controls/slider.cpp \
    modules/controls/spectrum.cpp \
    modules/controls/dockbar.cpp \
    modules/controls/window_title.cpp

HEADERS  += playo.h \
    dockbars.h \
    misc/conversions.h \
    misc/stylesheets.h \
    misc/filename_conversions.h \
    modules/json/data_store.h \
    modules/controls/tray.h \
    modules/controls/tabber.h \
    modules/controls/tab.h \
    modules/controls/tab_settings.h \
    modules/controls/views/index.h \
    modules/controls/views/view_types.h \
    modules/model_items/json_fields.h \
    modules/controls/json_fields.h \
    modules/models/_base_model.h \
    modules/model_items/items/_base_item.h \
    modules/model_items/items/item_interface.h \
    modules/models/model_interface.h \
    modules/model_items/item_state.h \
    modules/model_items/item_fields.h \
    modules/model_items/item_types.h \
    toolbars.h \
    modules/controls/toolbar.h \
    modules/controls/toolbarbutton.h \
    dialogs/toolbarbuttondialog.h \
    dialogs/toolbardialog.h \
    single_application.h \
    modules/controls/button.h \
    modules/controls/clickable_label.h \
    modules/controls/hoverable_label.h \
    misc/screen.h \
    modules/controls/window.h \
    modules/controls/window_title.h \
    dialogs/settings_dialog.h \
    dialogs/tabdialog.h \
    misc/settings.h \
    misc/hotkey_manager.h \
    misc/hotkey_types.h \
    misc/tab_settings.h \
    media/audio_player.h \
    media/player.h \
    media/duration.h \
    media/format.h \
    media/notify_timer.h \
    modules/models/hotkey_model/hotkey_delegate.h \
    modules/models/hotkey_model/hotkey_editor.h \
    modules/models/hotkey_model/hotkey_model.h \
    modules/models/hotkey_model/hotkey_model_item.h \
    globalshortcut/qxtglobal.h \
    globalshortcut/qxtglobalshortcut.h \
    globalshortcut/qxtglobalshortcut_p.h \
    modules/controls/popa.h \
    modules/controls/slider.h \
    modules/controls/slider_style.h \
    modules/controls/spectrum.h \
    modules/controls/dockbar.h

FORMS    += playo.ui \
    dialogs/toolbarbuttondialog.ui \
    dialogs/toolbardialog.ui \
    dialogs/settings_dialog.ui \
    dialogs/tabdialog.ui

RESOURCES += \
    resources.qrc

unix:!mac {
        QT += gui-private
        CONFIG += link_pkgconfig
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

LIBS += -L$$PWD/libs/bass/ -lbass -lbass_fx -lbassmix
INCLUDEPATH += $$PWD/libs/bass/include
DEPENDPATH += $$PWD/libs/bass
