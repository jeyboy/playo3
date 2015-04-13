#ifndef DROP_BUTTON
#define DROP_BUTTON

#include "hoverable_label.h"
#include "hoverable_menu.h"
#include <qmenu.h>
#include <QWidgetAction>

namespace Playo3 {
    class DropButton : public HoverableLabel {
        Q_OBJECT
    public:
        DropButton(int buttonHeight, QWidget * parent = 0);
        ~DropButton();

        void registerAction(QWidget * action);
    public slots:
        void hoverIn();
    protected:
        HoverableMenu * menu;
    };
}

#endif // DROP_BUTTON

