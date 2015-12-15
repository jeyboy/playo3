#ifndef TOOLBAR_USER_BUTTON_H
#define TOOLBAR_USER_BUTTON_H

#include <qfileinfo.h>

#include "toolbar_drop_button.h"
#include "modules/views/service/download_view.h"
#include "modules/views/view_interface.h"

namespace Controls {
    class ToolbarUserButton : public ToolbarDropButton {
        Q_OBJECT
    public:
        ToolbarUserButton(const QString & text, const QString & folderPath, QWidget * parent = 0);
        inline QString mainPath() const { return path; }
    protected slots:
        void checkState();
    protected:
        void dropReaction(QDropEvent * event);
        inline void enterEvent(QEvent *) { checkState(); }
    private:
        QString path;
    };
}

#endif // TOOLBAR_USER_BUTTON_H
