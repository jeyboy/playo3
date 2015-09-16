#include "library_settings.h"

void LibrarySettings::fromJson(const Json::Obj & json) {
    _remoteItemsProcDelay = json.value(SETTINGS_REMOTE_LIB_PROC_DELAY_KEY).toInt(1000); // in msec

    _saveLibDelay = json.value(SETTINGS_SAVE_LIB_DELAY_KEY).toInt(10000); // in msec

    _interactiveProc = json.value(SETTINGS_INTERACTIVE_LIB_PROC_KEY).toBool(false);
    _showInfo = json.value(SETTINGS_SHOW_INFO_KEY).toBool(true);
}

void LibrarySettings::toJson(Json::Obj & json) {
    json.insert(SETTINGS_REMOTE_LIB_PROC_DELAY_KEY, QJsonValue(_remoteItemsProcDelay));
    json.insert(SETTINGS_SAVE_LIB_DELAY_KEY, QJsonValue(_saveLibDelay));
    json.insert(SETTINGS_INTERACTIVE_LIB_PROC_KEY, QJsonValue(_interactiveProc));
    json.insert(SETTINGS_SHOW_INFO_KEY, QJsonValue(_showInfo));
}
