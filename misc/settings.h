#ifndef SETTINGS_H
#define SETTINGS_H

//#include <QJsonArray>
//#include <QJsonObject>
//#include <QApplication>
//#include <QMessageBox>

#include "settings/global_settings.h"
#include "settings/hotkey_settings.h"
#include "settings/item_settings.h"
#include "settings/spectrum_settings.h"
#include "settings/tab_settings.h"

class Settings : public GlobalSettings, public HotkeySettings,
        public ItemSettings, public SpectrumSettings, public TabSettings {
public:
    ~Settings() { }

    static Settings * instance();
    static void close() {
        delete self;
    }

    void fromJson(QJsonObject settingsObj = QJsonObject());
    QJsonObject toJson();

    inline int totalItemHeight() { return itemHeight() + (isShowInfo() ? itemInfoFontSize() * 2 : 0); }
    inline int iconHeight() { return totalItemHeight() - 1; }

private:
    Settings() : GlobalSettings(), HotkeySettings(),
        ItemSettings(), SpectrumSettings(), TabSettings() {
    }

    static Settings * self;
};

#endif // SETTINGS_H
