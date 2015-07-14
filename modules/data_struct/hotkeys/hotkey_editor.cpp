#include "hotkey_editor.h"
#include "QDebug"

void HotkeyEditor::keyReleaseEvent(QKeyEvent *) {
    emit editingFinished();

    if (!HotkeyManager::instance() -> registerSequence(keyType, keySequence().toString())) {
        qDebug() << "LOL";
    }
}
