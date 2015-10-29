#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include "modules/core/interfaces/singleton.h"
#include "player/player_index.h"

#include "settings/global_settings.h"
#include "settings/hotkey_settings.h"
#include "settings/item_settings.h"
#include "settings/spectrum_settings.h"
#include "settings/tab_settings.h"
#include "settings/library_settings.h"

class Settings : public GlobalSettings, public HotkeySettings,
        public ItemSettings, public SpectrumSettings,
        public TabSettings, public LibrarySettings, public Core::Singleton<Settings> {

    Settings(); friend class Core::Singleton<Settings>;
    QWidget * anchor;
    IPlayer * player;
public:
    void fromJson(QJsonObject settingsObj = QJsonObject());
    QJsonObject toJson();

    inline int totalItemHeight() { return itemHeight() + (isShowInfo() ? itemInfoFontSize() * 2 : 0); }
    inline int iconHeight() { return totalItemHeight() - 1; }

    inline QWidget * anchorWidget() { return anchor; }
    inline void anchorWidget(QWidget * newAnchor) { anchor = newAnchor; }

    inline IPlayer * currPlayer() { return player; }
    void currPlayer(const PlayerType & ptype) { player = PlayerFactory::obj().build(ptype); }

    void resetGlobalSettings();
    void resetHotkeySettings();
    void resetItemSettings();
    void resetSpectrumSettings();
    void resetTabSettings();
    void resetLibrarySettings();
};

#endif // APP_SETTINGS_H
