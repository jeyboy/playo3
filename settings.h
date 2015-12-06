#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <qdebug.h>
#include <qmainwindow.h>

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

//    static void setTransparency(QWidget * widget, bool activate = true) {
//        widget -> setAttribute(Qt::WA_NoSystemBackground, activate);
//        widget -> setAttribute(Qt::WA_TranslucentBackground, activate);
//        if (qobject_cast<QMainWindow *>(widget))
//            widget -> setAttribute(Qt::WA_OpaquePaintEvent, activate);

//        widget -> update();
//    }
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
//        bool is_transparent = currentStyle -> isTransparent();
        delete currentStyle;

        currentStyle = IStylesheets::createStylesheet(newType);

////        if (is_transparent != currentStyle -> isTransparent())
//            is_transparent = currentStyle -> isTransparent();
//            qDebug() << "TRANS" << is_transparent << transparences;
//            for(QList<QWidget *>::Iterator widget = transparences.begin(); widget != transparences.end(); widget++)
//                setTransparency(*widget, is_transparent);

        QApplication * app = ((QApplication *)QApplication::instance());
        app -> setStyleSheet(currentStyle -> appStyles());
    }

//    static void registerTransparentWidget(QWidget * widget) {
//        transparences.append(widget);
//        setTransparency(widget, currentStyle -> isTransparent());
//    }
//    static void unregisterTransparentWidget(QWidget * widget) { transparences.removeAll(widget); }

    static IStylesheets * currentStyle;
//    static QList<QWidget *> transparences;
};

#endif // APP_SETTINGS_H
