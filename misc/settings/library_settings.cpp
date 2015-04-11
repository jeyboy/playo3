#include "library_settings.h"

void LibrarySettings::fromJson(QJsonObject & settings) {
    _remoteItemsProcDelay = settings.value("remote_lib_proc_delay").toInt(2000); // in msec

    _saveLibDelay = settings.value("save_lib_delay").toInt(10000); // in msec

    _interactiveProc = settings.value("interactive_lib_proc").toBool(false);
    _showInfo = settings.value("show_info").toBool(true);
}

void LibrarySettings::toJson(QJsonObject & settings) {
    settings.insert("remote_lib_proc_delay", QJsonValue(_remoteItemsProcDelay));
    settings.insert("save_lib_delay", QJsonValue(_saveLibDelay));
    settings.insert("interactive_lib_proc", QJsonValue(_interactiveProc));
    settings.insert("show_info", QJsonValue(_showInfo));
}
