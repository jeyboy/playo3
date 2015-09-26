#ifndef HOTKEY_EDITOR_H
#define HOTKEY_EDITOR_H

#include <qkeysequenceedit.h>
#include <qevent.h>

#include "modules/models/service/hotkey_manager.h"

class HotkeyEditor : public QKeySequenceEdit  {
    Q_OBJECT
public:
    inline HotkeyEditor(int hotkeyType, QWidget * parent = 0) : QKeySequenceEdit(parent) { keyType = hotkeyType; }
signals:
    void editingFinished();
protected:
    void keyReleaseEvent(QKeyEvent *);
private:
    int keyType;
};

#endif // HOTKEY_EDITOR_H
