#include "tab_settings.h"

void TabSettings::fromJson(const QJsonObject & settings) {
    _showCheckbox = settings.value(SETTINGS_SHOW_CHECKBOX_KEY).toBool(true);

    _spoilOnActivation = settings.value(SETTINGS_SPOIL_ON_ACTIVATION_KEY).toBool(true);
    _alertOnFolderDeletion = settings.value(SETTINGS_ALERT_ON_FOLDER_DELETION_KEY).toBool(true);

    _showSystemIcons = settings.value(SETTINGS_SHOW_SYSTEM_ICON_KEY).toBool(false);

    _heightUnification = settings.value(SETTINGS_HEIGHT_UNIFICATION_KEY).toBool(true);

    _treeIndentation = settings.value(SETTINGS_TREE_IDENTATION_KEY).toInt(12);
    _item_present_type = settings.value(SETTINGS_ITEM_PRESENTATION_TYPE_KEY).toInt(2);

    _findValidOnFailure = settings.value(SETTINGS_FIND_VALID_KEY).toBool(true);
}

void TabSettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_SHOW_CHECKBOX_KEY, QJsonValue(_showCheckbox));

    settings.insert(SETTINGS_SPOIL_ON_ACTIVATION_KEY, QJsonValue(_spoilOnActivation));
    settings.insert(SETTINGS_ALERT_ON_FOLDER_DELETION_KEY, QJsonValue(_alertOnFolderDeletion));

    settings.insert(SETTINGS_SHOW_SYSTEM_ICON_KEY, QJsonValue(_showSystemIcons));
    settings.insert(SETTINGS_HEIGHT_UNIFICATION_KEY, QJsonValue(_heightUnification));

    settings.insert(SETTINGS_TREE_IDENTATION_KEY, QJsonValue(_treeIndentation));
    settings.insert(SETTINGS_ITEM_PRESENTATION_TYPE_KEY, QJsonValue(_item_present_type));

    settings.insert(SETTINGS_FIND_VALID_KEY, QJsonValue(_findValidOnFailure));
}
