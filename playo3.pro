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
    modules/controls/main_window.cpp

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
    modules/controls/main_window.h

FORMS    += playo.ui \
    dialogs/toolbarbuttondialog.ui \
    dialogs/toolbardialog.ui

RESOURCES += \
    resources.qrc
