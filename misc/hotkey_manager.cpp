#include "hotkey_manager.h"

HotkeyManager * HotkeyManager::self = 0;

HotkeyManager * HotkeyManager::instance(QObject * parent) {
    if(!self)
        self = new HotkeyManager(parent);
    return self;
}

QxtGlobalShortcut * HotkeyManager::registerSequence(const HotkeySlot & hotSlot) {
    QxtGlobalShortcut * shortcut = new QxtGlobalShortcut();
    connect(shortcut, SIGNAL(activated()), hotSlot.obj, hotSlot.slot);
    return shortcut;
}

bool HotkeyManager::updateSequence(QxtGlobalShortcut * shortcut, QKeySequence sequence) {
    QKeySequence oldSequence = shortcut -> shortcut();

    if (shortcut -> setShortcut(sequence))
        return true;
    else
        shortcut -> setShortcut(oldSequence);

    return false;
}

void HotkeyManager::clear() {
    qDeleteAll(shortcuts.values());
    shortcuts.clear();
}

bool HotkeyManager::registerSequence(int hotkeyType, QString sequence, QObject * receiver, const char * slot) {
    if (receiver != 0 && slot != 0)
        relations.insert(hotkeyType, HotkeySlot(receiver, slot));

    if (shortcuts.contains(hotkeyType))
        return updateSequence(shortcuts.value(hotkeyType), QKeySequence(sequence));
    else {
        if (relations.contains(hotkeyType)) {
            shortcuts.insert(hotkeyType, registerSequence(relations.value(hotkeyType)));
            return shortcuts.value(hotkeyType) -> setShortcut(QKeySequence(sequence));
        }
    }

    return false;
}
