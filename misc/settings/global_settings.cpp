#include "global_settings.h"

void GlobalSettings::fromJson(const Json::Obj & json) {
    _showMetric = json.value(SETTINGS_SHOW_METRICS_KEY).toBool(true);
    _showMetricNumero = json.value(SETTINGS_SHOW_METRICS_NUMERO_KEY).toBool(false);

    _defaultDownloadPath = json.value(SETTINGS_DOWNLOAD_PATH_KEY).toString(QCoreApplication::applicationDirPath() % QStringLiteral("/downloads/"));
    _tabPosition = json.value(SETTINGS_TAB_POSITION_KEY).toInt(0);
    _openTimeOut = json.value(SETTINGS_OPEN_TIMEOUT).toInt(15);

    _saveCommonTab = json.value(SETTINGS_SAVE_COMMON_TAB_KEY).toBool(false);
    _openDropPointInTab = json.value(SETTINGS_OPEN_DROP_IN_TAB_KEY).toBool(false);
    _openDropPointInTabType = (Playo3::ContainerType)json.value(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY).toInt(Playo3::tree);

    _isAutorunned = json.value(SETTINGS_AUTORUNNED_KEY).toBool(false);
}

void GlobalSettings::toJson(Json::Obj & json) {
    json.insert(SETTINGS_AUTORUNNED_KEY, QJsonValue::fromVariant(_isAutorunned));

    json.insert(SETTINGS_SHOW_METRICS_KEY, QJsonValue::fromVariant(_showMetric));
    json.insert(SETTINGS_SHOW_METRICS_NUMERO_KEY, QJsonValue::fromVariant(_showMetricNumero));
    json.insert(SETTINGS_DOWNLOAD_PATH_KEY, QJsonValue::fromVariant(_defaultDownloadPath));
    json.insert(SETTINGS_TAB_POSITION_KEY, QJsonValue::fromVariant(_tabPosition));

    json.insert(SETTINGS_SAVE_COMMON_TAB_KEY, QJsonValue::fromVariant(_saveCommonTab));
    json.insert(SETTINGS_OPEN_DROP_IN_TAB_KEY, QJsonValue::fromVariant(_openDropPointInTab));
    json.insert(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY, QJsonValue::fromVariant(_openDropPointInTabType));

    json.insert(SETTINGS_OPEN_TIMEOUT, QJsonValue::fromVariant(_openTimeOut));
}
