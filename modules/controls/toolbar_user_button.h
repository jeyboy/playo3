#ifndef TOOLBAR_USER_BUTTON_H
#define TOOLBAR_USER_BUTTON_H

#include <qfileinfo.h>

#include "toolbar_drop_button.h"
#include "modules/views/service/download_view.h"
#include "modules/views/view_interface.h"

namespace Controls {
    class ToolbarUserButton : public ToolbarDropButton {
        Q_OBJECT

        QString path;
    public:
        ToolbarUserButton(const QString & text, const QString & folderPath, QWidget * parent = 0);
        inline QString mainPath() const { return path; }
        inline void setMainPath(const QString & newPath) { path = newPath; }
    protected slots:
        void checkState();
    protected:
        void dropReaction(QDropEvent * event);
        inline void enterEvent(QEvent *) { checkState(); }
    };
}

#endif // TOOLBAR_USER_BUTTON_H
