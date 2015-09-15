#include "tab_settings.h"

void TabSettings::fromJson(const Json::Obj & json) {
    _showCheckbox = json.value(SETTINGS_SHOW_CHECKBOX_KEY).toBool(true);

    _spoilOnActivation = json.value(SETTINGS_SPOIL_ON_ACTIVATION_KEY).toBool(true);
    _alertOnFolderDeletion = json.value(SETTINGS_ALERT_ON_FOLDER_DELETION_KEY).toBool(true);

    _showSystemIcons = json.value(SETTINGS_SHOW_SYSTEM_ICON_KEY).toBool(false);

    _heightUnification = json.value(SETTINGS_HEIGHT_UNIFICATION_KEY).toBool(true);

    _treeIndentation = json.value(SETTINGS_TREE_IDENTATION_KEY).toInt(12);
    _item_present_type = json.value(SETTINGS_ITEM_PRESENTATION_TYPE_KEY).toInt(2);

    _findValidOnFailure = json.value(SETTINGS_FIND_VALID_KEY).toBool(true);
}

void TabSettings::toJson(Json::Obj & json) {
    json.insert(SETTINGS_SHOW_CHECKBOX_KEY, QJsonValue(_showCheckbox));

    json.insert(SETTINGS_SPOIL_ON_ACTIVATION_KEY, QJsonValue(_spoilOnActivation));
    json.insert(SETTINGS_ALERT_ON_FOLDER_DELETION_KEY, QJsonValue(_alertOnFolderDeletion));

    json.insert(SETTINGS_SHOW_SYSTEM_ICON_KEY, QJsonValue(_showSystemIcons));
    json.insert(SETTINGS_HEIGHT_UNIFICATION_KEY, QJsonValue(_heightUnification));

    json.insert(SETTINGS_TREE_IDENTATION_KEY, QJsonValue(_treeIndentation));
    json.insert(SETTINGS_ITEM_PRESENTATION_TYPE_KEY, QJsonValue(_item_present_type));

    json.insert(SETTINGS_FIND_VALID_KEY, QJsonValue(_findValidOnFailure));
}
