#ifndef SETTINGS_H
#define SETTINGS_H

#include "settings/global_settings.h"
#include "settings/hotkey_settings.h"
#include "settings/item_settings.h"
#include "settings/spectrum_settings.h"
#include "settings/tab_settings.h"
#include "settings/library_settings.h"

class Settings : public GlobalSettings, public HotkeySettings,
        public ItemSettings, public SpectrumSettings,
        public TabSettings, public LibrarySettings {
public:
    virtual ~Settings() { }

    static Settings * instance();
    inline static void close() { delete self; }

    void fromJson(const Json::Obj & settingsObj = Json::Obj());
    Json::Obj toJson();

    inline int totalItemHeight() { return itemHeight() + (isShowInfo() ? itemInfoFontSize() * 2 : 0); }
    inline int iconHeight() { return totalItemHeight() - 1; }

    inline void resetGlobalSettings()   { GlobalSettings::fromJson(); }
    inline void resetHotkeySettings()   { HotkeySettings::fromJson(); }
    inline void resetItemSettings()     { ItemSettings::fromJson(); }
    inline void resetSpectrumSettings() { SpectrumSettings::fromJson(); }
    inline void resetTabSettings()      { TabSettings::fromJson(); }
    inline void resetLibrarySettings()  { LibrarySettings::fromJson(); }

private:
    Settings() : GlobalSettings(), HotkeySettings(),
        ItemSettings(), SpectrumSettings(),
        TabSettings(), LibrarySettings() {
    }

    static Settings * self;
};

#endif // SETTINGS_H
