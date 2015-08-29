#include "global_settings.h"

void GlobalSettings::fromJson(QJsonObject & settings) {
    _showMetric = settings.value(SETTINGS_SHOW_METRICS_KEY).toBool(true);
    _showMetricNumero = settings.value(SETTINGS_SHOW_METRICS_NUMERO_KEY).toBool(false);

    _defaultDownloadPath = settings.value(SETTINGS_DOWNLOAD_PATH_KEY).toString(QCoreApplication::applicationDirPath() % QStringLiteral("/downloads/"));
    _tabPosition = settings.value(SETTINGS_TAB_POSITION_KEY).toInt(0);

    _saveCommonTab = settings.value(SETTINGS_SAVE_COMMON_TAB_KEY).toBool(false);
    _openDropPointInTab = settings.value(SETTINGS_OPEN_DROP_IN_TAB_KEY).toBool(false);
    _openDropPointInTabType = (Playo3::ContainerType)settings.value(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY).toInt(Playo3::tree);

    _isAutorunned = settings.value(SETTINGS_AUTORUNNED_KEY).toBool(false);

    _od_key = settings.value(SETTINGS_OD_KEY).toString();
    _od_val = settings.value(SETTINGS_OD_VAL_KEY).toString();
}

void GlobalSettings::toJson(QJsonObject & settings) {   
    settings.insert(SETTINGS_AUTORUNNED_KEY, QJsonValue::fromVariant(_isAutorunned));

    settings.insert(SETTINGS_SHOW_METRICS_KEY, QJsonValue::fromVariant(_showMetric));
    settings.insert(SETTINGS_SHOW_METRICS_NUMERO_KEY, QJsonValue::fromVariant(_showMetricNumero));
    settings.insert(SETTINGS_DOWNLOAD_PATH_KEY, QJsonValue::fromVariant(_defaultDownloadPath));
    settings.insert(SETTINGS_TAB_POSITION_KEY, QJsonValue::fromVariant(_tabPosition));

    settings.insert(SETTINGS_SAVE_COMMON_TAB_KEY, QJsonValue::fromVariant(_saveCommonTab));
    settings.insert(SETTINGS_OPEN_DROP_IN_TAB_KEY, QJsonValue::fromVariant(_openDropPointInTab));
    settings.insert(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY, QJsonValue::fromVariant(_openDropPointInTabType));

    settings.insert(SETTINGS_OD_KEY, QJsonValue::fromVariant(_od_key));
    settings.insert(SETTINGS_OD_VAL_KEY, QJsonValue::fromVariant(_od_val));
}
