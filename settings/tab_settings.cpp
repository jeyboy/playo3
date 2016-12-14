#include "tab_settings.h"

void TabSettings::fromJson(const QJsonObject & settings) {
    _show_checkbox = settings.value(SETTINGS_SHOW_CHECKBOX_KEY).toBool(true);

    _spoil_on_activation = settings.value(SETTINGS_SPOIL_ON_ACTIVATION_KEY).toBool(true);
    _alert_on_folder_deletion = settings.value(SETTINGS_ALERT_ON_FOLDER_DELETION_KEY).toBool(true);

    _show_system_icons = settings.value(SETTINGS_SHOW_SYSTEM_ICON_KEY).toBool(false);

    _height_unification = settings.value(SETTINGS_HEIGHT_UNIFICATION_KEY).toBool(true);

    _tree_indentation = settings.value(SETTINGS_TREE_IDENTATION_KEY).toInt(12);
    _item_present_type = settings.value(SETTINGS_ITEM_PRESENTATION_TYPE_KEY).toInt(2);

    _find_valid_on_failure = settings.value(SETTINGS_FIND_VALID_KEY).toBool(true);

    _show_number = settings.value(SETTINGS_SHOW_NUMBER_KEY).toBool(true);
}

void TabSettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_SHOW_CHECKBOX_KEY, QJsonValue(_show_checkbox));

    settings.insert(SETTINGS_SPOIL_ON_ACTIVATION_KEY, QJsonValue(_spoil_on_activation));
    settings.insert(SETTINGS_ALERT_ON_FOLDER_DELETION_KEY, QJsonValue(_alert_on_folder_deletion));

    settings.insert(SETTINGS_SHOW_SYSTEM_ICON_KEY, QJsonValue(_show_system_icons));
    settings.insert(SETTINGS_HEIGHT_UNIFICATION_KEY, QJsonValue(_height_unification));

    settings.insert(SETTINGS_TREE_IDENTATION_KEY, QJsonValue(_tree_indentation));
    settings.insert(SETTINGS_ITEM_PRESENTATION_TYPE_KEY, QJsonValue(_item_present_type));

    settings.insert(SETTINGS_FIND_VALID_KEY, QJsonValue(_find_valid_on_failure));

    settings.insert(SETTINGS_SHOW_NUMBER_KEY, QJsonValue(_show_number));
}
