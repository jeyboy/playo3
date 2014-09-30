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
    modules/model_items/model_item.cpp \
    modules/models/base_model.cpp \
    dockbars.cpp \
    modules/controls/tabber.cpp \
    modules/controls/tab.cpp

HEADERS  += playo.h \
    misc/conversions.h \
    modules/models/model_index.h \
    modules/models/base_model.h \
    modules/json/json_base.h \
    modules/model_items/model_item.h \
    modules/model_items/model_item_types.h \
    modules/model_items/model_item_fields.h \
    modules/json/data_store.h \
    modules/model_items/model_item_json_fields.h \
    modules/controls/control_json_fields.h \
    modules/model_items/model_item_state.h \
    dockbars.h \
    modules/controls/tray.h \
    modules/controls/tabber.h \
    misc/stylesheets.h \
    modules/controls/tab.h \
    modules/controls/view_types.h

FORMS    += playo.ui
