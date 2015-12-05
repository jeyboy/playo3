#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <qdebug.h>

#include "modules/core/interfaces/singleton.h"
#include "settings/stylesheets/stylesheets_list.h"

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
    ~Settings() { delete currentStyle; }
    QWidget * anchor;
public:
    void fromJson(QJsonObject settingsObj = QJsonObject());
    QJsonObject toJson();

    inline int totalItemHeight() { return itemHeight() + (isShowInfo() ? itemInfoFontSize() * 2 : 0); }
    inline int iconHeight() { return totalItemHeight() - 1; }

    inline QWidget * anchorWidget() { return anchor; }
    inline void anchorWidget(QWidget * newAnchor) { anchor = newAnchor; }

    void resetGlobalSettings();
    void resetHotkeySettings();
    void resetItemSettings();
    void resetSpectrumSettings();
    void resetTabSettings();
    void resetLibrarySettings();

    static void setCurrentStyle(IStylesheets::StyleType newType) {
        delete currentStyle;
        currentStyle = IStylesheets::createStylesheet(newType);
        qDebug() << "SET" << currentStyle -> styleType();
        QApplication * app = ((QApplication *)QApplication::instance());
        qDebug() << app;
        qDebug() << currentStyle -> appStyles();
        app -> setStyleSheet(currentStyle -> appStyles());
    }

    static IStylesheets * currentStyle;
};

#endif // APP_SETTINGS_H
