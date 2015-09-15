#ifndef HOTKEY_SETTINGS
#define HOTKEY_SETTINGS

#include "modules/data_struct/hotkeys/hotkey_model_item.h"
#include "misc/hotkey_types.h"
#include "misc/web_utils/json.h"

#define JSON_KEY QStringLiteral("hotkeys")

class HotkeySettings {
public:
    HotkeySettings();

    void fromJson(const Json::Obj & json = Json::Obj());
    void toJson(QJsonObject & settings);

    QList<HotkeyModelItem *> * hotKeys() const;
    void setHotKeys(QList<HotkeyModelItem *>);
    void setHotKeys(const Json::Obj & hotkeysHash = Json::Obj());

protected:
    Json::Obj _hotkeys;
    QHash<int, QString> _humanizeHotkeyText;
};

#endif // HOTKEY_SETTINGS
