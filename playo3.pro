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
    modules/model_items/items/base_item.cpp \
    modules/models/base_model.cpp \
    modules/controls/tabber.cpp \
    modules/controls/tab.cpp \
    dockbars.cpp

HEADERS  += playo.h \
    dockbars.h \
    misc/conversions.h \
    misc/stylesheets.h \
    misc/filename_conversions.h \
    modules/models/model_index.h \
    modules/models/base_model.h \
    modules/json/data_store.h \
    modules/controls/tray.h \
    modules/controls/tabber.h \
    modules/controls/tab.h \
    modules/controls/tab_settings.h \
    modules/controls/views/index.h \
    modules/controls/views/view_types.h \
    modules/model_items/items/base_item.h \
    modules/model_items/fields.h \
    modules/model_items/json_fields.h \
    modules/model_items/state.h \
    modules/model_items/types.h \
    modules/controls/json_fields.h

FORMS    += playo.ui
