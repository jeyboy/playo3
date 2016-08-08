#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T00:45:08
#
#-------------------------------------------------

VERSION = 0.6.7.0

CONFIG += c++11
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
#DEFINES *= QT_USE_QSTRINGBUILDER

QT  += core gui network
#QT       += multimedia multimediawidgets opengl

win32: {
    QT  += winextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SUBDIRS += tests # uncomment for tests running

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
    dialogs/user_action_dialog.ui \
    dialogs/packages_dialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = playo.rc

unix:!mac {
        QT += gui-private
        CONFIG += link_pkgconfig
        PKGCONFIG += x11
        LDFLAGS =- lX11
        DEFINES += HAVE_X11
        SOURCES += modules/controls/qxtglobalshortcut_x11.cpp
        SOURCES += modules/core/misc/file_utils/watcher_x11.cpp
}
win32: {
    SOURCES += modules/controls/qxtglobalshortcut_win.cpp
    SOURCES += modules/core/misc/file_utils/watcher_win.cpp
}
mac: {
    SOURCES += modules/controls/qxtglobalshortcut_mac.cpp
    SOURCES += modules/core/misc/file_utils/watcher_mac.cpp
}

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
    modules/controls/tray.h \
    modules/controls/user_dialog_box.h \
    modules/controls/window.h \
    modules/controls/window_title.h \
    modules/core/core_parts/part_mixes/item_fields.h \
    modules/core/core_parts/part_mixes/item_state.h \
    modules/core/core_parts/part_mixes/item_types.h \
    modules/core/core_parts/part_mixes/json_fields.h \
    modules/core/core_parts/item_drop_formats.h \
    modules/core/core_parts/playlist.h \
    modules/core/web/grabber_keys.h \
    modules/core/web/interfaces/igrabber_api.h \
    modules/core/interfaces/isearchable.h \
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
    modules/core/web/utils/html_parser.h \
    modules/core/web/utils/unicode_decoding.h \
    modules/core/web/utils/web_manager.h \
    modules/core/web/web_apis.h \
    modules/core/data_factory.h \
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
    modules/models/web/ignore_list.h \
    modules/models/web/od_model.h \
    modules/models/web/soundcloud_model.h \
    modules/models/web/vk_model.h \
    modules/models/web/web_model.h \
    modules/models/level_tree_model.h \
    modules/models/list_model.h \
    modules/models/model_interface.h \
    modules/models/model_item_delegate.h \
    modules/models/tree_model.h \
    modules/views/web/od_view.h \
    modules/views/web/soundcloud_view.h \
    modules/views/web/vk_view.h \
    modules/views/level_tree_view.h \
    modules/views/list_view.h \
    modules/views/tree_view.h \
    modules/views/tree_view_style.h \
    modules/views/view_index.h \
    modules/views/view_interface.h \
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
    toolbars.h \
    settings.h \
    modules/core/media/interfaces/itrackable.h \
    modules/core/media/interfaces/iplayer.h \
    modules/core/media/interfaces/ispectrumable.h \
    modules/core/media/interfaces/player_states.h \
    modules/core/media/interfaces/iequalizable.h \
    modules/core/media/interfaces/player_statuses.h \
    modules/core/interfaces/singleton.h \
    modules/core/misc/file_utils/file_errors.h \
    modules/core/web/response_error.h \
    player/bass_player.h \
    modules/core/misc/thread_utils.h \
    modules/core/misc/func.h \
    modules/core/misc/thread_cell.h \
    player/player_index.h \
    modules/core/media/interfaces/imediainfo.h \
    modules/core/interfaces/iplaylistable.h \
    player/player_callback.h \
    modules/controls/time_label.h \
    modules/core/web/interfaces/sociable/friendable.h \
    modules/core/web/interfaces/sociable/groupable.h \
    modules/core/web/interfaces/sociable/sociable.h \
    modules/core/web/interfaces/sociable/linkable.h \
    modules/core/web/interfaces/sociable/linkable_list.h \
    modules/models/service/relations_delegate.h \
    modules/core/web/utils/html_parser_keys.h \
    settings/stylesheets/dark_stylesheets.h \
    settings/stylesheets/light_stylesheets.h \
    settings/stylesheets/istylesheets.h \
    settings/stylesheets/stylesheets_list.h \
    dialogs/base_dialog.h \
    modules/controls/toolbar_drop_button.h \
    modules/controls/toolbar_user_button.h \
    modules/controls/toolbar_new_list_button.h \
    modules/controls/search_configurator.h \
    modules/core/misc/fuzzy_comparison.h \
    modules/core/misc/file_utils/filesystem_watcher.h \
    modules/core/media/library_base.h \
    modules/pizduck/duk_config.h \
    modules/pizduck/duktape.h \
    modules/core/core_parts/part_mixes/item_errors.h \
    modules/core/core_parts/part_mixes/item_field_defines.h \
    modules/core/data_sub_types.h \
    modules/core/core_parts/item.h \
    settings/apis_settings.h \
    modules/models/model_settings.h \
    modules/core/interfaces/isource.h \
    modules/core/interfaces/igenreable.h \
    modules/core/web/interfaces/iqueriable.h \
    modules/core/web/interfaces/queriable_arg.h \
    modules/core/web/utils/js_document.h \
    modules/core/web/interfaces/quariable_defines.h \
    modules/core/feeder/aspect_ratio_label.h \
    modules/core/feeder/feed_widget.h \
    modules/core/feeder/feeder.h \
    modules/core/media/image_bank.h \
    modules/core/interfaces/search_limits.h \
    modules/core/web/interfaces/auth/oauth.h \
    modules/core/interfaces/isource_auth_perm.h \
    modules/core/interfaces/isource_feeds.h \
    modules/core/feeder/feed_item.h \
    modules/views/web/fourshared_view.h \
    modules/models/web/fourshared_model.h \
    modules/core/web/utils/web_headers.h \
    modules/core/interfaces/iuser_interaction.h \
    modules/core/interfaces/isource_perm_flags.h \
    dialogs/packages_dialog.h \
    modules/core/web/utils/web_connection.h \
    modules/core/web/services/recaptcha.h \
    modules/core/web/sources/soundcloud/soundcloud_auth.h \
    modules/core/web/sources/soundcloud/soundcloud_defines.h \
    modules/core/web/sources/soundcloud/soundcloud_group.h \
    modules/core/web/sources/soundcloud/soundcloud_keys.h \
    modules/core/web/sources/soundcloud/soundcloud_playlist.h \
    modules/core/web/sources/soundcloud/soundcloud_requests.h \
    modules/core/web/sources/soundcloud/soundcloud_set.h \
    modules/core/web/sources/soundcloud/soundcloud_track.h \
    modules/core/web/sources/soundcloud/soundcloud_user.h \
    modules/core/web/sources/vk/vk_auth.h \
    modules/core/web/sources/vk/vk_defines.h \
    modules/core/web/sources/vk/vk_feed.h \
    modules/core/web/sources/vk/vk_group.h \
    modules/core/web/sources/vk/vk_keys.h \
    modules/core/web/sources/vk/vk_playlist.h \
    modules/core/web/sources/vk/vk_requests.h \
    modules/core/web/sources/vk/vk_set.h \
    modules/core/web/sources/vk/vk_track.h \
    modules/core/web/sources/vk/vk_user.h \
    modules/core/web/sources/vk/vk_video.h \
    modules/core/web/sources/fourshared/fourshared_api_keys.h \
    modules/core/web/sources/fourshared/fourshared_defines.h \
    modules/core/web/sources/jetune.h \
    modules/core/web/sources/mp3base.h \
    modules/core/web/sources/mp3cc.h \
    modules/core/web/sources/mp3pm.h \
    modules/core/web/sources/musicshara.h \
    modules/core/web/sources/myzuka_album.h \
    modules/core/web/sources/promodj.h \
    modules/core/web/sources/redmp3.h \
    modules/core/web/sources/shmidt.h \
    modules/core/web/sources/soundcloud_queries.h \
    modules/core/web/sources/vk_queries.h \
    modules/core/web/sources/yandex_api.h \
    modules/core/web/sources/yandex/yandex_api_keys.h \
    modules/core/web/sources/yandex/yandex_request_api.h \
    modules/core/web/sources/youtube_api.h \
    modules/core/web/sources/youtube/youtube_api_keys.h \
    modules/core/web/sources/youtube/youtube_api_misc.h \
    modules/core/web/sources/youtube/youtube_request_api.h \
    modules/core/web/sources/zaycev.h \
    modules/core/web/sources/_sources.h \
    modules/core/web/sources/fourshared/fourshared_set.h \
    modules/core/web/sources/fourshared/fourshared_item.h \
    modules/core/web/sources/fourshared/fourshared_requests.h \
    modules/core/web/sources/fourshared/fourshared_auth.h \
    modules/core/web/sources/od/od_auth.h \
    modules/core/web/sources/od/od_defines.h \
    modules/core/web/sources/od/od_track.h \
    modules/core/web/sources/od/od_artist.h \
    modules/core/web/sources/od/od_set.h \
    modules/core/web/sources/od/od_video.h \
    modules/core/web/sources/od/od_video_playlist.h \
    modules/core/web/sources/od/od_playlist.h \
    modules/core/web/sources/od/od_requests.h \
    modules/core/web/sources/od/od_keys.h \
    modules/core/web/sources/od/od_misc.h \
    modules/core/web/sources/od/od_collection.h \
    modules/core/web/sources/od/od_user.h \
    modules/core/web/sources/od_queries.h \
    modules/core/web/sources/od/od_group.h \
    modules/core/web/sources/fourshared_queries.h \
    modules/core/web/sources/od/od_tuner.h \
    modules/core/interfaces/icmd.h \
    modules/core/interfaces/icmd_fields.h \
    modules/core/web/sources/fourshared/fourshared_track.h \
    modules/core/web/sources/fourshared/fourshared_video.h

SOURCES += \
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
    modules/controls/user_dialog_box.cpp \
    modules/controls/window.cpp \
    modules/controls/window_title.cpp \
    modules/core/core_parts/part_mixes/item_fields.cpp \
    modules/core/core_parts/part_mixes/item_state.cpp \
    modules/core/core_parts/playlist.cpp \
    modules/core/web/grabber_keys.cpp \
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
    modules/core/web/utils/html_parser.cpp \
    modules/core/web/utils/web_manager.cpp \
    modules/core/web/web_apis.cpp \
    modules/core/data_factory.cpp \
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
    modules/models/web/web_model.cpp \
    modules/models/level_tree_model.cpp \
    modules/models/list_model.cpp \
    modules/models/model_interface.cpp \
    modules/models/model_item_delegate.cpp \
    modules/models/tree_model.cpp \
    modules/views/web/od_view.cpp \
    modules/views/web/soundcloud_view.cpp \
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
    main.cpp \
    playo.cpp \
    single_application.cpp \
    toolbars.cpp \
    settings.cpp \
    modules/core/media/interfaces/itrackable.cpp \
    modules/core/media/interfaces/iplayer.cpp \
    modules/core/media/interfaces/ispectrumable.cpp \
    modules/core/media/interfaces/iequalizable.cpp \
    player/bass_player.cpp \
    external_keys.cpp \
    modules/core/web/utils/html_parser_keys.cpp \
    modules/models/service/relations_delegate.cpp \
    settings/stylesheets/dark_stylesheets.cpp \
    settings/stylesheets/light_stylesheets.cpp \
    settings/stylesheets/istylesheets.cpp \
    modules/controls/toolbar_drop_button.cpp \
    modules/controls/toolbar_user_button.cpp \
    modules/controls/toolbar_new_list_button.cpp \
    modules/controls/search_configurator.cpp \
    modules/core/misc/file_utils/filesystem_watcher.cpp \
    modules/core/media/library_base.cpp \
    modules/pizduck/duktape.c \
    modules/core/core_parts/item.cpp \
    settings/apis_settings.cpp \
    modules/core/interfaces/isource.cpp \
    modules/core/feeder/aspect_ratio_label.cpp \
    modules/core/interfaces/search_limits.cpp \
    modules/core/interfaces/isource_auth_perm.cpp \
    modules/views/web/fourshared_view.cpp \
    modules/models/web/fourshared_model.cpp \
    modules/core/interfaces/iuser_interaction.cpp \
    dialogs/packages_dialog.cpp \
    modules/core/web/utils/unicode_decoding.cpp \
    modules/core/web/utils/web_connection.cpp \
    modules/core/web/sources/soundcloud/soundcloud_keys.cpp \
    modules/core/web/sources/soundcloud/soundcloud_requests.cpp \
    modules/core/web/sources/vk/vk_keys.cpp \
    modules/core/web/sources/exua.cpp \
    modules/core/web/sources/fourshared/fourshared_api_keys.cpp \
    modules/core/web/sources/jetune.cpp \
    modules/core/web/sources/mp3base.cpp \
    modules/core/web/sources/mp3cc.cpp \
    modules/core/web/sources/mp3pm.cpp \
    modules/core/web/sources/musicshara.cpp \
    modules/core/web/sources/myzuka_album.cpp \
    modules/core/web/sources/promodj.cpp \
    modules/core/web/sources/redmp3.cpp \
    modules/core/web/sources/shmidt.cpp \
    modules/core/web/sources/soundcloud_queries.cpp \
    modules/core/web/sources/vk_queries.cpp \
    modules/core/web/sources/yandex/yandex_api_keys.cpp \
    modules/core/web/sources/youtube_api.cpp \
    modules/core/web/sources/youtube/youtube_api_keys.cpp \
    modules/core/web/sources/zaycev.cpp \
    modules/core/web/sources/od/od_keys.cpp \
    modules/core/web/sources/od_queries.cpp \
    modules/core/web/sources/fourshared_queries.cpp
