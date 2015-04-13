#include "global_settings.h"

void GlobalSettings::fromJson(QJsonObject & settings) {
    _showMetric = settings.value("show_metric").toBool(true);
    _defaultDownloadPath = settings.value("download_path").toString(QCoreApplication::applicationDirPath() + "/downloads/");
    _tabPosition = settings.value("tab_position").toInt(0);

    _saveCommonTab = settings.value("save_common_tab").toBool(false);
    _openDropPointInTab = settings.value("open_drop_point_in_tab").toBool(false);
    _openDropPointInTabType = (Playo3::ContainerType)settings.value("open_drop_point_in_tab_type").toInt(Playo3::tree);
}

void GlobalSettings::toJson(QJsonObject & settings) {
    settings.insert("show_metric", QJsonValue::fromVariant(_showMetric));
    settings.insert("download_path", QJsonValue::fromVariant(_defaultDownloadPath));
    settings.insert("tab_position", QJsonValue::fromVariant(_tabPosition));

    settings.insert("save_common_tab", QJsonValue::fromVariant(_saveCommonTab));
    settings.insert("open_drop_point_in_tab", QJsonValue::fromVariant(_openDropPointInTab));
    settings.insert("open_drop_point_in_tab_type", QJsonValue::fromVariant(_openDropPointInTabType));
}
