#include "tab_settings.h"

void TabSettings::fromJson(QJsonObject & settings) {
    _showCheckbox = settings.value("show_checkboxes").toBool(true);

    _spoilOnActivation = settings.value("spoil_on_activation").toBool(true);
    _alertOnFolderDeletion = settings.value("alert_on_folder_deletion").toBool(true);

    _showInfo = settings.value("show_info").toBool(true);
    _showSystemIcons = settings.value("show_system_icons").toBool(false);

    _treeIndentation = settings.value("tree_indentation").toInt(12);
}

void TabSettings::toJson(QJsonObject & settings) {
    settings.insert("show_checkboxes", QJsonValue(_showCheckbox));

    settings.insert("spoil_on_activation", QJsonValue(_spoilOnActivation));
    settings.insert("alert_on_folder_deletion", QJsonValue(_alertOnFolderDeletion));

    settings.insert("show_info", QJsonValue(_showInfo));
    settings.insert("show_system_icons", QJsonValue(_showSystemIcons));

    settings.insert("tree_indentation", QJsonValue(_treeIndentation));
}
