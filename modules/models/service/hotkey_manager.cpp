#include "hotkey_manager.h"

QxtGlobalShortcut * HotkeyManager::registerSequence(const HotkeySlot & hotSlot) {
    QxtGlobalShortcut * shortcut = new QxtGlobalShortcut();
    QObject::connect(shortcut, SIGNAL(activated()), hotSlot.obj, hotSlot.slot);
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

bool HotkeyManager::registerSequence(int hotkeyType, const QString & sequence, QObject * receiver, const char * slot) {
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
