#include "library_settings.h"

void LibrarySettings::fromJson(const QJsonObject & settings) {
    _remoteItemsProcDelay = settings.value(SETTINGS_REMOTE_LIB_PROC_DELAY_KEY).toInt(1000); // in msec

    _saveLibDelay = settings.value(SETTINGS_SAVE_LIB_DELAY_KEY).toInt(10000); // in msec

    _interactiveProc = settings.value(SETTINGS_INTERACTIVE_LIB_PROC_KEY).toBool(false);
    _showInfo = settings.value(SETTINGS_SHOW_INFO_KEY).toBool(true);
}

void LibrarySettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_REMOTE_LIB_PROC_DELAY_KEY, QJsonValue(_remoteItemsProcDelay));
    settings.insert(SETTINGS_SAVE_LIB_DELAY_KEY, QJsonValue(_saveLibDelay));
    settings.insert(SETTINGS_INTERACTIVE_LIB_PROC_KEY, QJsonValue(_interactiveProc));
    settings.insert(SETTINGS_SHOW_INFO_KEY, QJsonValue(_showInfo));
}
