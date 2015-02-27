#include "global_settings.h"

void GlobalSettings::fromJson(QJsonObject & settings) {
    _showMetric = settings.value("show_metric").toBool(true);
    _defaultDownloadPath = settingsObj.value("download_path").toString(QCoreApplication::applicationDirPath() + "/downloads/");
}

void GlobalSettings::toJson(QJsonObject & settings) {
    settings.insert("show_metric", QJsonValue::fromVariant(_showMetric));
    settings.insert("download_path", QJsonValue::fromVariant(_defaultDownloadPath));
}
