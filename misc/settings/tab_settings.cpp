#include "tab_settings.h"

void TabSettings::fromJson(QJsonObject & settings) {
    _showCheckbox = settings.value("show_checkboxes").toBool(true);

    _spoilOnActivation = settings.value("spoil_on_activation").toBool(true);
    _alertOnFolderDeletion = settings.value("alert_on_folder_deletion").toBool(true);

    _showInfo = settings.value("show_info").toBool(true);
    _showSystemIcons = settings.value("show_system_icons").toBool(false);

    _tabPosition = settings.value("tab_position").toInt(0);
    _treeIndentation = settings.value("tree_indentation").toInt(12);
}

void TabSettings::toJson(QJsonObject & settings) {
    settings.insert("show_checkboxes", QJsonValue::fromVariant(_showCheckbox));

    settings.insert("spoil_on_activation", QJsonValue::fromVariant(_spoilOnActivation));
    settings.insert("alert_on_folder_deletion", QJsonValue::fromVariant(_alertOnFolderDeletion));

    settings.insert("show_info", QJsonValue::fromVariant(_showInfo));
    settings.insert("show_system_icons", QJsonValue::fromVariant(_showSystemIcons));

    settings.insert("tab_position", QJsonValue::fromVariant(_tabPosition));

    settings.insert("tree_indentation", QJsonValue::fromVariant(_treeIndentation));
}
