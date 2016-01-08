#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H

#include <qhash.h>

#include "modules/core/interfaces/singleton.h"
#include "modules/controls/qxtglobalshortcut.h"
#include "hotkey_types.h"

struct HotkeySlot {
    const QObject * obj;
    const char * slot;

    HotkeySlot(const QObject * _obj = 0, const char * _slot = 0) {
        obj = _obj;
        slot = _slot;
    }
};

class HotkeyManager : public Core::Singleton<HotkeyManager> {
    friend class Core::Singleton<HotkeyManager>;
    HotkeyManager() {}

    QxtGlobalShortcut * registerSequence(const HotkeySlot & hotSlot);
    bool updateSequence(QxtGlobalShortcut * shortcut, QKeySequence sequence);

    QHash<int, HotkeySlot> relations;
    QHash<int, QxtGlobalShortcut *> shortcuts;
public:
    inline virtual ~HotkeyManager() {
        shortcuts.clear();
//        foreach (QxtGlobalShortcut * hotkey, shortcuts.values())
//            delete hotkey;
    }

    bool registerSequence(int hotkeyType, const QString & sequence, QObject * receiver = 0, const char * slot = 0);
    void clear();
private:

};

#endif // HOTKEY_MANAGER_H
