#include "settings.h"

IStylesheets * Settings::currentStyle = new IStylesheets();
//QList<QWidget *> Settings::transparences = QList<QWidget *>();

Settings::Settings() : anchor(0) {}

void Settings::fromJson(QJsonObject settings) {
    GlobalSettings::fromJson(settings);
    ApisSettings::fromJson(settings);
    HotkeySettings::fromJson(settings);
    ItemSettings::fromJson(settings);
    SpectrumSettings::fromJson(settings);
    TabSettings::fromJson(settings);
    LibrarySettings::fromJson(settings);
}

QJsonObject Settings::toJson() {
    QJsonObject settings = QJsonObject();

    GlobalSettings::toJson(settings);
    ApisSettings::toJson(settings);
    HotkeySettings::toJson(settings);
    ItemSettings::toJson(settings);
    SpectrumSettings::toJson(settings);
    TabSettings::toJson(settings);
    LibrarySettings::toJson(settings);

    return settings;
}

void Settings::resetGlobalSettings()   { GlobalSettings::fromJson(QJsonObject()); }
void Settings::resetApisSettings()     { ApisSettings::fromJson(QJsonObject()); }
void Settings::resetHotkeySettings()   { HotkeySettings::fromJson(QJsonObject()); }
void Settings::resetItemSettings()     { ItemSettings::fromJson(QJsonObject()); }
void Settings::resetSpectrumSettings() { SpectrumSettings::fromJson(QJsonObject()); }
void Settings::resetTabSettings()      { TabSettings::fromJson(QJsonObject()); }
void Settings::resetLibrarySettings()  { LibrarySettings::fromJson(QJsonObject()); }
