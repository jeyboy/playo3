#include "global_settings.h"

void GlobalSettings::fromJson(QJsonObject & settings) {
    _showMetric = settings.value("show_metric").toBool(true);
    _defaultDownloadPath = settings.value("download_path").toString(QCoreApplication::applicationDirPath() + "/downloads/");
    _tabPosition = settings.value("tab_position").toInt(0);
}

void GlobalSettings::toJson(QJsonObject & settings) {
    settings.insert("show_metric", QJsonValue::fromVariant(_showMetric));
    settings.insert("download_path", QJsonValue::fromVariant(_defaultDownloadPath));
    settings.insert("tab_position", QJsonValue::fromVariant(_tabPosition));
}
