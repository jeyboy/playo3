#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

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

    void fromJson(QJsonObject settingsObj = QJsonObject());
    QJsonObject toJson();

    inline int totalItemHeight() { return itemHeight() + (isShowInfo() ? itemInfoFontSize() * 2 : 0); }
    inline int iconHeight() { return totalItemHeight() - 1; }

    inline void resetGlobalSettings()   { GlobalSettings::fromJson(QJsonObject()); }
    inline void resetHotkeySettings()   { HotkeySettings::fromJson(QJsonObject()); }
    inline void resetItemSettings()     { ItemSettings::fromJson(QJsonObject()); }
    inline void resetSpectrumSettings() { SpectrumSettings::fromJson(QJsonObject()); }
    inline void resetTabSettings()      { TabSettings::fromJson(QJsonObject()); }
    inline void resetLibrarySettings()  { LibrarySettings::fromJson(QJsonObject()); }

private:
    Settings() : GlobalSettings(), HotkeySettings(),
        ItemSettings(), SpectrumSettings(),
        TabSettings(), LibrarySettings() {
    }

    static Settings * self;
};

#endif // APP_SETTINGS_H
