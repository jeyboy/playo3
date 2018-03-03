#include "global_settings.h"

void GlobalSettings::fromJson(const QJsonObject & settings) {
    _show_metric_marks = settings.value(SETTINGS_SHOW_METRICS_KEY).toBool(true);
    _show_metric_numero = settings.value(SETTINGS_SHOW_METRICS_NUMERO_KEY).toBool(false);

    _default_download_path = settings.value(SETTINGS_DOWNLOAD_PATH_KEY).toString(SETTINGS_DEFAULT_DOWNLOAD_PATH);
    _tab_position = settings.value(SETTINGS_TAB_POSITION_KEY).toInt(0);

    _save_common_tab = settings.value(SETTINGS_SAVE_COMMON_TAB_KEY).toBool(false);
    _clear_common_tab_on_exit = settings.value(SETTINGS_CLEAR_COMMON_TAB_KEY).toBool(true);

    _open_drop_point_in_tab = settings.value(SETTINGS_OPEN_DROP_IN_TAB_KEY).toBool(false);
    _open_drop_point_in_tab_type = (Core::DataSubType)settings.value(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY).toInt(Core::dt_tree);

    _is_autorunned = settings.value(SETTINGS_AUTORUNNED_KEY).toBool(false);
    _color_scheme = settings.value(SETTINGS_COLOR_SCHEME_KEY).toInt(2);

    _tool_icon_size = settings.value(SETTINGS_TOOL_ICON_SIZE_KEY).toInt(28);
    _output_device = settings.value(SETTINGS_OUTPUT_DEVICE_KEY).toString();
    _show_download = settings.value(SETTINGS_SHOW_DOWNLOAD_KEY).toBool(true);

    _tray_show_played = settings.value(SETTINGS_TRAY_SHOW_PLAYED_KEY).toInt(tsi_inform);
    _tray_show_period = settings.value(SETTINGS_TRAY_SHOW_PERIOD_KEY).toInt(10000);
}

void GlobalSettings::toJson(QJsonObject & settings) {   
    settings.insert(SETTINGS_AUTORUNNED_KEY, QJsonValue::fromVariant(_is_autorunned));

    settings.insert(SETTINGS_SHOW_METRICS_KEY, QJsonValue::fromVariant(_show_metric_marks));
    settings.insert(SETTINGS_SHOW_METRICS_NUMERO_KEY, QJsonValue::fromVariant(_show_metric_numero));
    settings.insert(SETTINGS_DOWNLOAD_PATH_KEY, QJsonValue::fromVariant(_default_download_path));
    settings.insert(SETTINGS_TAB_POSITION_KEY, QJsonValue::fromVariant(_tab_position));

    settings.insert(SETTINGS_SAVE_COMMON_TAB_KEY, QJsonValue::fromVariant(_save_common_tab));
    settings.insert(SETTINGS_CLEAR_COMMON_TAB_KEY, QJsonValue::fromVariant(_clear_common_tab_on_exit));

    settings.insert(SETTINGS_OPEN_DROP_IN_TAB_KEY, QJsonValue::fromVariant(_open_drop_point_in_tab));
    settings.insert(SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY, QJsonValue::fromVariant(_open_drop_point_in_tab_type));

    settings.insert(SETTINGS_COLOR_SCHEME_KEY, QJsonValue::fromVariant(_color_scheme));

    settings.insert(SETTINGS_TOOL_ICON_SIZE_KEY, QJsonValue::fromVariant(_tool_icon_size));
    settings.insert(SETTINGS_OUTPUT_DEVICE_KEY, QJsonValue::fromVariant(_output_device));

    settings.insert(SETTINGS_SHOW_DOWNLOAD_KEY, QJsonValue::fromVariant(_show_download));

    settings.insert(SETTINGS_TRAY_SHOW_PLAYED_KEY, QJsonValue::fromVariant(_tray_show_played));
    settings.insert(SETTINGS_TRAY_SHOW_PERIOD_KEY, QJsonValue::fromVariant(_tray_show_period));
}
