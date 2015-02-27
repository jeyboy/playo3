#ifndef HOTKEY_SETTINGS
#define HOTKEY_SETTINGS

#include <QJsonArray>
#include <QJsonObject>

#include "modules/data_struct/hotkeys/hotkey_model_item.h"
#include "misc/hotkey_types.h"

class HotkeySettings {
public:
    HotkeySettings();

    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    QList<HotkeyModelItem *> * hotKeys() const;
    void setHotKeys(QList<HotkeyModelItem *>);
    void setHotKeys(QJsonObject hotkeysHash = QJsonObject());

protected:
//    inline QJsonObject hotkeysJson() const { return _hotkeys; }

    QJsonObject _hotkeys;
    QHash<int, QString> _humanizeHotkeyText;
};

#endif // HOTKEY_SETTINGS
