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
        SOURCES += modules/controls/qxtglobalshortcut_x11.cpp
}
win32: { SOURCES += modules/controls/qxtglobalshortcut_win.cpp }
mac: {   SOURCES += modules/controls/qxtglobalshortcut_mac.cpp }

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

HEADERS += \
    dialogs/echonest_dialog.h \
    dialogs/extension_dialog.h \
    dialogs/relations_dialog.h \
    dialogs/search_dialog.h \
    dialogs/settings_dialog.h \
    dialogs/tabdialog.h \
    dialogs/user_action_dialog.h \
    modules/controls/accordion.h \
    modules/controls/clickable_label.h \
    modules/controls/clickable_slider.h \
    modules/controls/dockbar.h \
    modules/controls/drop_button.h \
    modules/controls/equalizer_view.h \
    modules/controls/hoverable_label.h \
    modules/controls/hoverable_menu.h \
    modules/controls/json_fields.h \
    modules/controls/metric_slider.h \
    modules/controls/qxtglobal.h \
    modules/controls/qxtglobalshortcut.h \
    modules/controls/qxtglobalshortcut_p.h \
    modules/controls/qxtnamespace.h \
    modules/controls/qxtspanslider.h \
    modules/controls/qxtspanslider_p.h \
    modules/controls/rotate_label.h \
    modules/controls/search_bar.h \
    modules/controls/simple_list_view.h \
    modules/controls/slider_style.h \
    modules/controls/spectrum_view.h \
    modules/controls/spinner.h \
    modules/controls/title_layout.h \
    modules/controls/toolbar.h \
    modules/controls/toolbarbutton.h \
    modules/controls/tray.h \
    modules/controls/user_dialog_box.h \
    modules/controls/window.h \
    modules/controls/window_title.h \
    modules/core/core_parts/part_mixes/item_fields.h \
    modules/core/core_parts/part_mixes/item_state.h \
    modules/core/core_parts/part_mixes/item_types.h \
    modules/core/core_parts/part_mixes/json_fields.h \
    modules/core/core_parts/core_part_types.h \
    modules/core/core_parts/file.h \
    modules/core/core_parts/item_drop_formats.h \
    modules/core/core_parts/item_interface.h \
    modules/core/core_parts/od_file.h \
    modules/core/core_parts/od_playlist.h \
    modules/core/core_parts/playlist.h \
    modules/core/core_parts/soundcloud_file.h \
    modules/core/core_parts/soundcloud_playlist.h \
    modules/core/core_parts/vk_file.h \
    modules/core/core_parts/vk_playlist.h \
    modules/core/core_parts/web_file.h \
    modules/core/core_parts/web_playlist.h \
    modules/core/interfaces/grabber_keys.h \
    modules/core/interfaces/iapi.h \
    modules/core/interfaces/igrabber_api.h \
    modules/core/interfaces/isearchable.h \
    modules/core/interfaces/web_api.h \
    modules/core/media/cue/cue.h \
    modules/core/media/cue/cue_structs.h \
    modules/core/media/genres/genres_interface.h \
    modules/core/media/genres/id3_music_genres.h \
    modules/core/media/genres/music_genres.h \
    modules/core/media/genres/target_genres.h \
    modules/core/media/genres/vk_genres.h \
    modules/core/media/library.h \
    modules/core/media/mediainfo.h \
    modules/core/misc/file_utils/data_store.h \
    modules/core/misc/file_utils/extensions.h \
    modules/core/misc/file_utils/filename_conversions.h \
    modules/core/misc/file_utils/icon_provider.h \
    modules/core/misc/autorun.h \
    modules/core/misc/format.h \
    modules/core/misc/logger.h \
    modules/core/misc/screen.h \
    modules/core/web/apis/service/_services.h \
    modules/core/web/apis/service/echonest_api.h \
    modules/core/web/apis/service/echonest_artist_api.h \
    modules/core/web/apis/service/echonest_genre_api.h \
    modules/core/web/apis/service/echonest_playlist_api.h \
    modules/core/web/apis/service/echonest_song_api.h \
    modules/core/web/apis/site/_sites.h \
    modules/core/web/apis/site/exua.h \
    modules/core/web/apis/site/jetune.h \
    modules/core/web/apis/site/mp3base.h \
    modules/core/web/apis/site/mp3cc.h \
    modules/core/web/apis/site/mp3pm.h \
    modules/core/web/apis/site/musicshara.h \
    modules/core/web/apis/site/myzuka_album.h \
    modules/core/web/apis/site/promodj.h \
    modules/core/web/apis/site/shmidt.h \
    modules/core/web/apis/site/zaycev.h \
    modules/core/web/apis/social/_socials.h \
    modules/core/web/apis/social/od_api.h \
    modules/core/web/apis/social/od_api_keys.h \
    modules/core/web/apis/social/od_api_misc.h \
    modules/core/web/apis/social/od_request_api.h \
    modules/core/web/apis/social/soundcloud_api.h \
    modules/core/web/apis/social/soundcloud_api_keys.h \
    modules/core/web/apis/social/soundcloud_request_api.h \
    modules/core/web/apis/social/vk_api.h \
    modules/core/web/apis/social/vk_api_keys.h \
    modules/core/web/apis/social/vk_request_api.h \
    modules/core/web/apis/storage/_storages.h \
    modules/core/web/apis/storage/fourshared_api.h \
    modules/core/web/apis/storage/fourshared_api_keys.h \
    modules/core/web/apis/storage/fourshared_request_api.h \
    modules/core/web/auth_chemas/teu_auth.h \
    modules/core/web/utils/html_parser.h \
    modules/core/web/utils/json_parser.h \
    modules/core/web/utils/unicode_decoding.h \
    modules/core/web/utils/web_manager.h \
    modules/core/web/web_apis.h \
    modules/core/web/web_sub_types.h \
    modules/core/core_parts_index.h \
    modules/core/data_core.h \
    modules/core/playlist_types.h \
    modules/models/service/download_delegate.h \
    modules/models/service/download_model.h \
    modules/models/service/download_model_item.h \
    modules/models/service/hotkey_delegate.h \
    modules/models/service/hotkey_editor.h \
    modules/models/service/hotkey_manager.h \
    modules/models/service/hotkey_model.h \
    modules/models/service/hotkey_model_item.h \
    modules/models/service/hotkey_types.h \
    modules/models/service/search_model.h \
    modules/models/service/search_settings.h \
    modules/models/web/ignore_list.h \
    modules/models/web/od_model.h \
    modules/models/web/soundcloud_model.h \
    modules/models/web/vk_model.h \
    modules/models/web/vk_rel_model.h \
    modules/models/web/web_model.h \
    modules/models/level_tree_model.h \
    modules/models/list_model.h \
    modules/models/model_interface.h \
    modules/models/model_item_delegate.h \
    modules/models/tree_model.h \
    modules/plugins/plugins.h \
    modules/plugins/web_dialog_interface.h \
    modules/views/web/od_view.h \
    modules/views/web/soundcloud_view.h \
    modules/views/web/vk_rel_view.h \
    modules/views/web/vk_view.h \
    modules/views/level_tree_view.h \
    modules/views/list_view.h \
    modules/views/tree_view.h \
    modules/views/tree_view_style.h \
    modules/views/view_index.h \
    modules/views/view_interface.h \
    modules/views/view_settings.h \
    modules/views/service/download_view.h \
    modules/views/service/search_view.h \
    settings/global_settings.h \
    settings/hotkey_settings.h \
    settings/item_settings.h \
    settings/library_settings.h \
    settings/spectrum_settings.h \
    settings/tab_settings.h \
    dockbars.h \
    external_keys.h \
    playo.h \
    single_application.h \
    stylesheets.h \
    toolbars.h \
    settings.h \
    modules/core/media/interfaces/itrackable.h \
    modules/core/media/interfaces/iplayer.h \
    modules/core/media/interfaces/ispectrumable.h \
    modules/core/media/interfaces/player_states.h \
    modules/core/media/interfaces/iequalizable.h \
    modules/core/media/interfaces/player_statuses.h \
    player/player.h \
    modules/core/interfaces/singleton.h \
    modules/core/misc/file_utils/file_errors.h \
    modules/core/web/response_error.h \
    player/bass_player.h \
    modules/core/misc/thread_utils.h \
    modules/core/misc/func.h \
    modules/core/misc/thread_cell.h \
    player/player_index.h \
    modules/core/media/interfaces/imediainfo.h \
    modules/core/interfaces/iplaylistable.h

SOURCES += \
    dialogs/echonest_dialog.cpp \
    dialogs/extension_dialog.cpp \
    dialogs/relations_dialog.cpp \
    dialogs/search_dialog.cpp \
    dialogs/settings_dialog.cpp \
    dialogs/tabdialog.cpp \
    dialogs/user_action_dialog.cpp \
    modules/controls/accordion.cpp \
    modules/controls/clickable_label.cpp \
    modules/controls/clickable_slider.cpp \
    modules/controls/dockbar.cpp \
    modules/controls/drop_button.cpp \
    modules/controls/equalizer_view.cpp \
    modules/controls/hoverable_label.cpp \
    modules/controls/metric_slider.cpp \
    modules/controls/qxtglobalshortcut.cpp \
    modules/controls/qxtspanslider.cpp \
    modules/controls/rotate_label.cpp \
    modules/controls/search_bar.cpp \
    modules/controls/simple_list_view.cpp \
    modules/controls/spectrum_view.cpp \
    modules/controls/spinner.cpp \
    modules/controls/toolbar.cpp \
    modules/controls/toolbarbutton.cpp \
    modules/controls/user_dialog_box.cpp \
    modules/controls/window.cpp \
    modules/controls/window_title.cpp \
    modules/core/core_parts/part_mixes/item_fields.cpp \
    modules/core/core_parts/part_mixes/item_state.cpp \
    modules/core/core_parts/file.cpp \
    modules/core/core_parts/item_interface.cpp \
    modules/core/core_parts/od_file.cpp \
    modules/core/core_parts/od_playlist.cpp \
    modules/core/core_parts/playlist.cpp \
    modules/core/core_parts/soundcloud_file.cpp \
    modules/core/core_parts/soundcloud_playlist.cpp \
    modules/core/core_parts/vk_file.cpp \
    modules/core/core_parts/vk_playlist.cpp \
    modules/core/core_parts/web_file.cpp \
    modules/core/core_parts/web_playlist.cpp \
    modules/core/interfaces/grabber_keys.cpp \
    modules/core/interfaces/web_api.cpp \
    modules/core/media/cue/cue.cpp \
    modules/core/media/genres/genres_interface.cpp \
    modules/core/media/genres/id3_music_genres.cpp \
    modules/core/media/genres/music_genres.cpp \
    modules/core/media/genres/vk_genres.cpp \
    modules/core/media/library.cpp \
    modules/core/media/mediainfo.cpp \
    modules/core/misc/file_utils/data_store.cpp \
    modules/core/misc/file_utils/extensions.cpp \
    modules/core/misc/file_utils/icon_provider.cpp \
    modules/core/misc/format.cpp \
    modules/core/misc/logger.cpp \
    modules/core/web/apis/service/echonest_api.cpp \
    modules/core/web/apis/site/exua.cpp \
    modules/core/web/apis/site/jetune.cpp \
    modules/core/web/apis/site/mp3base.cpp \
    modules/core/web/apis/site/mp3cc.cpp \
    modules/core/web/apis/site/mp3pm.cpp \
    modules/core/web/apis/site/musicshara.cpp \
    modules/core/web/apis/site/myzuka_album.cpp \
    modules/core/web/apis/site/promodj.cpp \
    modules/core/web/apis/site/shmidt.cpp \
    modules/core/web/apis/site/zaycev.cpp \
    modules/core/web/apis/social/od_api.cpp \
    modules/core/web/apis/social/od_api_keys.cpp \
    modules/core/web/apis/social/soundcloud_api.cpp \
    modules/core/web/apis/social/soundcloud_api_keys.cpp \
    modules/core/web/apis/social/vk_api.cpp \
    modules/core/web/apis/social/vk_api_keys.cpp \
    modules/core/web/apis/storage/fourshared_api.cpp \
    modules/core/web/apis/storage/fourshared_api_keys.cpp \
    modules/core/web/auth_chemas/teu_auth.cpp \
    modules/core/web/utils/html_parser.cpp \
    modules/core/web/utils/json_parser.cpp \
    modules/core/web/utils/web_manager.cpp \
    modules/core/web/web_apis.cpp \
    modules/core/data_core.cpp \
    modules/models/service/download_delegate.cpp \
    modules/models/service/download_model.cpp \
    modules/models/service/download_model_item.cpp \
    modules/models/service/hotkey_delegate.cpp \
    modules/models/service/hotkey_editor.cpp \
    modules/models/service/hotkey_manager.cpp \
    modules/models/service/hotkey_model.cpp \
    modules/models/service/hotkey_model_item.cpp \
    modules/models/service/search_model.cpp \
    modules/models/web/ignore_list.cpp \
    modules/models/web/od_model.cpp \
    modules/models/web/soundcloud_model.cpp \
    modules/models/web/vk_model.cpp \
    modules/models/web/vk_rel_model.cpp \
    modules/models/web/web_model.cpp \
    modules/models/level_tree_model.cpp \
    modules/models/list_model.cpp \
    modules/models/model_interface.cpp \
    modules/models/model_item_delegate.cpp \
    modules/models/tree_model.cpp \
    modules/plugins/plugins.cpp \
    modules/views/web/od_view.cpp \
    modules/views/web/soundcloud_view.cpp \
    modules/views/web/vk_rel_view.cpp \
    modules/views/web/vk_view.cpp \
    modules/views/level_tree_view.cpp \
    modules/views/list_view.cpp \
    modules/views/tree_view.cpp \
    modules/views/view_interface.cpp \
    modules/views/service/download_view.cpp \
    modules/views/service/search_view.cpp \
    settings/global_settings.cpp \
    settings/hotkey_settings.cpp \
    settings/item_settings.cpp \
    settings/library_settings.cpp \
    settings/spectrum_settings.cpp \
    settings/tab_settings.cpp \
    dockbars.cpp \
    external_keys.cpp \
    main.cpp \
    playo.cpp \
    single_application.cpp \
    stylesheets.cpp \
    toolbars.cpp \
    settings.cpp \
    modules/core/media/interfaces/itrackable.cpp \
    modules/core/media/interfaces/iplayer.cpp \
    modules/core/media/interfaces/ispectrumable.cpp \
    modules/core/media/interfaces/iequalizable.cpp \
    player/player.cpp \
    player/bass_player.cpp
