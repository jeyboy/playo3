#include "global_settings.h"

void GlobalSettings::fromJson(const QJsonObject & settings) {
    _showMetric = settings.value(SETTINGS_SHOW_METRICS_KEY).toBool(true);
    _showMetricNumero = settings.value(SETTINGS_SHOW_METRICS_NUMERO_KEY).toBool(false);

    _defaultDownloadPath = settings.value(SETTINGS_DOWNLOAD_PATH_KEY).toString(QCoreApplication::applicationDirPath() % QStringLiteral("/downloads/"));
    _tabPosition = settings.value(SETTINGS_TAB_POSITION_KEY).toInt(0);

    _saveCommonTab = settings.value(SETTINGS_SAVE_COMMON_TAB_KEY).toBool(false);
    _openDropPointInTab = settings.value(SETTINGS_OPEN_DROP_IN_TAB_KEY).toBool(false);
    _openDropPointInTabType = (Core::DataSubType)settings.value(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY).toInt(Core::dt_tree);

    _isAutorunned = settings.value(SETTINGS_AUTORUNNED_KEY).toBool(false);
    _colorScheme = settings.value(SETTINGS_COLOR_SCHEME_KEY).toInt(1);

    _toolIconSize = settings.value(SETTINGS_TOOL_ICON_SIZE_KEY).toInt(28);
    _output_device = settings.value(SETTINGS_OUTPUT_DEVICE_KEY).toString();
    _showDownload = settings.value(SETTINGS_SHOW_DOWNLOAD_KEY).toBool(true);

    _tray_show_played = settings.value(SETTINGS_TRAY_SHOW_PLAYED_KEY).toInt(tsi_inform);
    _tray_show_period = settings.value(SETTINGS_TRAY_SHOW_PERIOD_KEY).toInt(10000);
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

    settings.insert(SETTINGS_COLOR_SCHEME_KEY, QJsonValue::fromVariant(_colorScheme));

    settings.insert(SETTINGS_TOOL_ICON_SIZE_KEY, QJsonValue::fromVariant(_toolIconSize));
    settings.insert(SETTINGS_OUTPUT_DEVICE_KEY, QJsonValue::fromVariant(_output_device));

    settings.insert(SETTINGS_SHOW_DOWNLOAD_KEY, QJsonValue::fromVariant(_showDownload));

    settings.insert(SETTINGS_TRAY_SHOW_PLAYED_KEY, QJsonValue::fromVariant(_tray_show_played));
    settings.insert(SETTINGS_TRAY_SHOW_PERIOD_KEY, QJsonValue::fromVariant(_tray_show_period));
}
