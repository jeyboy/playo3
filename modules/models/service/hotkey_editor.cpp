#include "hotkey_editor.h"
#include "QDebug"

void HotkeyEditor::keyReleaseEvent(QKeyEvent *) {
    emit editingFinished();

    if (!HotkeyManager::obj().registerSequence(keyType, keySequence().toString())) {
        qDebug() << "LOL";
    }
}
