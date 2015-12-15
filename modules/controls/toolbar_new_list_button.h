#ifndef TOOLBAR_NEW_LIST_BUTTON_H
#define TOOLBAR_NEW_LIST_BUTTON_H

#include "toolbar_drop_button.h"

namespace Controls {
    class ToolbarNewListButton : public ToolbarDropButton {
        Q_OBJECT
    public:
        ToolbarNewListButton(const QIcon & ico, QWidget * parent = 0);
    signals:
        void folderAdded(QString name, QUrl url);
    protected:
        void dropReaction(QDropEvent * event);
    };
}

#endif // TOOLBAR_NEW_LIST_BUTTON_H
