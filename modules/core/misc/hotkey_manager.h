#ifndef HOTKEY_MANAGER_H
#define HOTKEY_MANAGER_H

#include <qhash.h>
#include "globalshortcut/qxtglobalshortcut.h"
#include "misc/hotkey_types.h"

struct HotkeySlot {
    const QObject * obj;
    const char * slot;

    HotkeySlot(const QObject * _obj = 0, const char * _slot = 0) {
        obj = _obj;
        slot = _slot;
    }
};

class HotkeyManager : public QObject {
    Q_OBJECT
public:
    inline virtual ~HotkeyManager() { qDeleteAll(shortcuts.values()); }

    static HotkeyManager * instance(QObject * parent = 0);

    inline static void close() { delete self; }

    bool registerSequence(int hotkeyType, QString sequence, QObject * receiver = 0, const char * slot = 0);
    void clear();

private:
    inline HotkeyManager(QObject * parent) : QObject(parent) { }

    QxtGlobalShortcut * registerSequence(const HotkeySlot & hotSlot);
    bool updateSequence(QxtGlobalShortcut * shortcut, QKeySequence sequence);

    static HotkeyManager * self;

    QHash<int, HotkeySlot> relations;
    QHash<int, QxtGlobalShortcut *> shortcuts;
};

#endif // HOTKEY_MANAGER_H
