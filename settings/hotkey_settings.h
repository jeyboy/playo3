#ifndef HOTKEY_SETTINGS
#define HOTKEY_SETTINGS

#include <qjsonarray.h>
#include <qjsonobject.h>

#include "modules/models/service/hotkey_model_item.h"
#include "modules/models/service/hotkey_types.h"

#define SETTINGS_DISABLE_HOTKEYS_KEY QStringLiteral("disable_hotkeys")
#define SETTINGS_HOTKEYS_KEY QStringLiteral("hotkeys")

class HotkeySettings {
public:
    HotkeySettings();

    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    QList<HotkeyModelItem *> hotKeys() const;
    void setHotKeys(QList<HotkeyModelItem *>);
    void setHotKeys(const QJsonObject & hotkeysHash = QJsonObject());

    void disableHotkeys(const bool & disable) { _disabled = disable; }
    bool isHotkeysDisabled() { return _disabled; }

protected:
    bool _disabled;
    QJsonObject _hotkeys;
    QHash<int, QString> _humanize_hotkey_text;
};

#endif // HOTKEY_SETTINGS
