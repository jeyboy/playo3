#include "settings.h"

Settings * Settings::self = 0;

Settings * Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

void Settings::fromJson(const Json::Obj & settings) {
    GlobalSettings::fromJson(settings);
    HotkeySettings::fromJson(settings);
    ItemSettings::fromJson(settings);
    SpectrumSettings::fromJson(settings);
    TabSettings::fromJson(settings);
    LibrarySettings::fromJson(settings);
}

Json::Obj Settings::toJson() {
    Json::Obj json = Json::Obj();

    GlobalSettings::toJson(json);
    HotkeySettings::toJson(json);
    ItemSettings::toJson(json);
    SpectrumSettings::toJson(json);
    TabSettings::toJson(json);
    LibrarySettings::toJson(json);

    return json;
}
