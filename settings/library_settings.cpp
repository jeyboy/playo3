#include "library_settings.h"

void LibrarySettings::fromJson(const QJsonObject & settings) {
    _remoteItemsProcDelay = settings.value(SETTINGS_REMOTE_LIB_PROC_DELAY_KEY).toInt(1000); // in msec

    _saveLibDelay = settings.value(SETTINGS_SAVE_LIB_DELAY_KEY).toInt(10000); // in msec

    _initiate_on_play = settings.value(SETTINGS_INITIATE_ON_PLAYING_KEY).toBool(true);
    _showInfo = settings.value(SETTINGS_SHOW_INFO_KEY).toBool(true);
}

void LibrarySettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_REMOTE_LIB_PROC_DELAY_KEY, QJsonValue(_remoteItemsProcDelay));
    settings.insert(SETTINGS_SAVE_LIB_DELAY_KEY, QJsonValue(_saveLibDelay));
    settings.insert(SETTINGS_INITIATE_ON_PLAYING_KEY, QJsonValue(_initiate_on_play));
    settings.insert(SETTINGS_SHOW_INFO_KEY, QJsonValue(_showInfo));
}
