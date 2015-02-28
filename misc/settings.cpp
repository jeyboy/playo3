#include "settings.h"

Settings * Settings::self = 0;

Settings * Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

void Settings::fromJson(QJsonObject settings) {
    GlobalSettings::fromJson(settings);
    HotkeySettings::fromJson(settings);
    ItemSettings::fromJson(settings);
    SpectrumSettings::fromJson(settings);
    TabSettings::fromJson(settings);
}

QJsonObject Settings::toJson() {
    QJsonObject settings = QJsonObject();

    GlobalSettings::toJson(settings);
    HotkeySettings::toJson(settings);
    ItemSettings::toJson(settings);
    SpectrumSettings::toJson(settings);
    TabSettings::toJson(settings);

    return settings;
}
