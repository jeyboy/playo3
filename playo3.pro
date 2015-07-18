#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T00:45:08
#
#-------------------------------------------------

VERSION = 0.5.4.1

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

SOURCES += main.cpp\
    single_application.cpp \
    playo.cpp \
    dockbars.cpp \
    toolbars.cpp \
    modules/controls/toolbar.cpp \
    modules/controls/toolbarbutton.cpp \
    dialogs/toolbarbuttondialog.cpp \
    dialogs/toolbardialog.cpp \
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
    globalshortcut/qxtglobalshortcut.cpp \
    modules/controls/spectrum.cpp \
    modules/controls/dockbar.cpp \
    modules/controls/window_title.cpp \
    modules/controls/window.cpp \
    misc/stylesheets.cpp \
    modules/controls/clickable_slider.cpp \
    modules/controls/metric_slider.cpp \
    modules/data_struct/hotkeys/hotkey_delegate.cpp \
    modules/data_struct/hotkeys/hotkey_editor.cpp \
    modules/data_struct/hotkeys/hotkey_model.cpp \
    modules/data_struct/hotkeys/hotkey_model_item.cpp \
    modules/data_struct/level_tree/level_tree_model.cpp \
    modules/data_struct/level_tree/level_tree_view.cpp \
    modules/data_struct/list/list_model.cpp \
    modules/data_struct/list/list_view.cpp \
    modules/data_struct/tree/tree_model.cpp \
    modules/data_struct/tree/tree_view.cpp \
    modules/data_struct/web/soundcloud/soundcloud_model.cpp \
    modules/data_struct/web/soundcloud/soundcloud_view.cpp \
    modules/data_struct/web/vk/vk_folder.cpp \
    modules/data_struct/web/vk/vk_model.cpp \
    modules/data_struct/web/vk/vk_view.cpp \
    modules/data_struct/web/web_model.cpp \
    modules/data_struct/model_interface.cpp \
    modules/data_struct/view_interface.cpp \
    modules/data_struct/model_item_parts/item_state.cpp \
    modules/data_struct/item_interface.cpp \
    misc/file_utils/data_store.cpp \
    modules/data_struct/file_item.cpp \
    modules/data_struct/folder_item.cpp \
    misc/file_utils/extensions.cpp \
    misc/file_utils/icon_provider.cpp \
    modules/data_struct/model_item_parts/item_fields.cpp \
    dialogs/captchadialog.cpp \
    dialogs/extension_dialog.cpp \
    modules/controls/simple_list_view.cpp \
    modules/data_struct/model_item_delegate.cpp \
    modules/controls/spinner.cpp \
    modules/controls/user_dialog_box.cpp \
    misc/settings/hotkey_settings.cpp \
    misc/settings/item_settings.cpp \
    misc/settings/global_settings.cpp \
    misc/settings/spectrum_settings.cpp \
    misc/settings/tab_settings.cpp \
    modules/data_struct/downloads/download_delegate.cpp \
    modules/data_struct/downloads/download_model.cpp \
    modules/data_struct/downloads/download_model_item.cpp \
    modules/data_struct/downloads/download_view.cpp \
    media/library.cpp \
    media/mediainfo.cpp \
    media/genres/genres_interface.cpp \
    media/genres/music_genres.cpp \
    media/genres/web/vk_genres.cpp \
    misc/settings/library_settings.cpp \
    modules/web/auth_chemas/teu_auth.cpp \
    modules/web/socials_api/soundcloud_api.cpp \
    modules/web/socials_api/vk_api.cpp \
    modules/web/web_api.cpp \
    modules/data_struct/web/web_item.cpp \
    modules/data_struct/web/web_folder_item.cpp \
    modules/data_struct/web/soundcloud/soundcloud_item.cpp \
    modules/data_struct/web/vk/vk_item.cpp \
    dialogs/relations_dialog.cpp \
    modules/data_struct/web/ignore_list.cpp \
    modules/controls/drop_button.cpp \
    modules/controls/rotate_label.cpp \
    misc/logger.cpp \
    modules/data_struct/web/soundcloud/soundcloud_folder.cpp \
    dialogs/search_dialog.cpp \
    modules/data_struct/search/search_model.cpp \
    modules/data_struct/search/search_view.cpp \
    modules/data_struct/web/vk/vk_rel_model.cpp \
    modules/data_struct/web/vk/vk_rel_view.cpp \
    modules/controls/search_bar.cpp \
    dialogs/export_dialog.cpp \
    media/cue/cue.cpp \
    modules/controls/equalizer.cpp \
    modules/web/service_api/echonest_api.cpp \
    dialogs/echonest_dialog.cpp \
    modules/controls/accordion.cpp \
    double_slider/qxtspanslider.cpp \
    misc/web_utils/html_parser.cpp \
    modules/web/sites/myzuka_album.cpp \
    misc/web_utils/web_manager.cpp \
    external_keys.cpp \
    modules/web/socials_api/soundcloud_api_keys.cpp \
    modules/web/socials_api/vk_api_keys.cpp \
    modules/web/sites/grabber_keys.cpp

HEADERS  += playo.h \
    single_application.h \
    toolbars.h \
    dockbars.h \
    misc/conversions.h \
    misc/stylesheets.h \
    modules/controls/tray.h \
    modules/controls/json_fields.h \
    modules/controls/toolbar.h \
    modules/controls/toolbarbutton.h \
    dialogs/toolbarbuttondialog.h \
    dialogs/toolbardialog.h \
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
    media/audio_player.h \
    media/player.h \
    media/duration.h \
    media/format.h \
    media/notify_timer.h \
    globalshortcut/qxtglobalshortcut.h \
    globalshortcut/qxtglobalshortcut_p.h \
    modules/controls/slider_style.h \
    modules/controls/spectrum.h \
    modules/controls/dockbar.h \
    modules/controls/title_layout.h \
    modules/controls/clickable_slider.h \
    modules/controls/metric_slider.h \
    modules/data_struct/hotkeys/hotkey_delegate.h \
    modules/data_struct/hotkeys/hotkey_editor.h \
    modules/data_struct/hotkeys/hotkey_model.h \
    modules/data_struct/hotkeys/hotkey_model_item.h \
    modules/data_struct/level_tree/level_tree_model.h \
    modules/data_struct/level_tree/level_tree_view.h \
    modules/data_struct/list/list_model.h \
    modules/data_struct/list/list_view.h \
    modules/data_struct/tree/tree_model.h \
    modules/data_struct/tree/tree_view.h \
    modules/data_struct/web/soundcloud/soundcloud_model.h \
    modules/data_struct/web/soundcloud/soundcloud_view.h \
    modules/data_struct/web/vk/vk_folder.h \
    modules/data_struct/web/vk/vk_model.h \
    modules/data_struct/web/vk/vk_view.h \
    modules/data_struct/web/web_model.h \
    modules/data_struct/model_interface.h \
    modules/data_struct/view_interface.h \
    modules/data_struct/model_item_parts/item_fields.h \
    modules/data_struct/model_item_parts/item_state.h \
    modules/data_struct/model_item_parts/item_types.h \
    modules/data_struct/model_item_parts/json_fields.h \
    modules/data_struct/item_interface.h \
    modules/data_struct/tree_view_style.h \
    misc/file_utils/data_store.h \
    modules/data_struct/view_settings.h \
    modules/data_struct/file_item.h \
    modules/data_struct/folder_item.h \
    modules/data_struct/view_index.h \
    modules/data_struct/item_index.h \
    misc/file_utils/extensions.h \
    misc/file_utils/filename_conversions.h \
    misc/file_utils/icon_provider.h \
    dialogs/captchadialog.h \
    dialogs/extension_dialog.h \
    modules/controls/simple_list_view.h \
    modules/data_struct/model_item_delegate.h \
    misc/spectrum_types.h \
    modules/data_struct/container_types.h \
    modules/controls/spinner.h \
    modules/controls/user_dialog_box.h \
    misc/settings/global_settings.h \
    misc/settings/hotkey_settings.h \
    misc/settings/item_settings.h \
    misc/settings/spectrum_settings.h \
    misc/settings/tab_settings.h \
    modules/data_struct/downloads/download_delegate.h \
    modules/data_struct/downloads/download_model.h \
    modules/data_struct/downloads/download_model_item.h \
    modules/data_struct/downloads/download_view.h \
    modules/data_struct/item_drop_formats.h \
    media/library.h \
    media/mediainfo.h \
    media/genres/web/vk_genres.h \
    media/genres/genres_interface.h \
    media/genres/music_genres.h \
    misc/settings/library_settings.h \
    modules/web/auth_chemas/teu_auth.h \
    modules/web/socials_api/soundcloud_api.h \
    modules/web/socials_api/vk_api.h \
    modules/web/web_api.h \
    modules/data_struct/web/web_item.h \
    modules/data_struct/web/web_folder_item.h \
    modules/data_struct/web/vk/vk_item.h \
    modules/data_struct/web/soundcloud/soundcloud_item.h \
    dialogs/relations_dialog.h \
    modules/data_struct/web/ignore_list.h \
    modules/controls/drop_button.h \
    modules/controls/hoverable_menu.h \
    modules/controls/rotate_label.h \
    misc/logger.h \
    modules/data_struct/web/soundcloud/soundcloud_folder.h \
    dialogs/search_dialog.h \
    modules/data_struct/search/search_model.h \
    modules/data_struct/search/search_settings.h \
    modules/data_struct/search/search_view.h \
    modules/data_struct/web/vk/vk_rel_model.h \
    modules/data_struct/web/vk/vk_rel_view.h \
    modules/data_struct/web/vk/vk_rel_types.h \
    modules/controls/search_bar.h \
    misc/autorun.h \
    dialogs/export_dialog.h \
    modules/web/web_apis.h \
    media/cue/cue_structs.h \
    media/cue/cue.h \
    modules/web/service_api/echonest_api.h \
    modules/web/service_api/echonest_genre_api.h \
    modules/controls/equalizer.h \
    modules/web/service_api/echonest_artist_api.h \
    modules/web/service_api/echonest_song_api.h \
    modules/plugins/web_dialog_interface.h \
    modules/plugins/loader.h \
    modules/web/service_api/echonest_playlist_api.h \
    dialogs/echonest_dialog.h \
    modules/controls/accordion.h \
    double_slider/qxtspanslider.h \
    double_slider/qxtspanslider_p.h \
    double_slider/qxtglobal.h \
    modules/web/iapi.h \
    modules/web/socials_api/soundcloud_request_api.h \
    misc/web_utils/json.h \
    modules/web/utils/async.h \
    modules/web/socials_api/vk_request_api.h \
    modules/web/sites/igrabber_api.h \
    modules/web/sites/myzuka_album.h \
    media/genres/web/target_genres.h \
    misc/web_utils/html_parser.h \
    misc/web_utils/web_manager.h \
    external_keys.h \
    modules/web/socials_api/soundcloud_api_keys.h \
    modules/web/socials_api/vk_api_keys.h \
    modules/web/sites/grabber_keys.h

FORMS    += playo.ui \
    dialogs/toolbarbuttondialog.ui \
    dialogs/toolbardialog.ui \
    dialogs/settings_dialog.ui \
    dialogs/tabdialog.ui \
    dialogs/captchadialog.ui \
    dialogs/extension_dialog.ui \
    dialogs/relations_dialog.ui \
    dialogs/search_dialog.ui \
    dialogs/export_dialog.ui \
    dialogs/echonest_dialog.ui

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
