#include "drop_button.h"
#include <qapplication.h>

using namespace Controls;

DropButton::DropButton(int button_height, QWidget * parent) :
    HoverableLabel(
        "",
        QPixmap(QStringLiteral(":/controls/drop_off")).scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap(QStringLiteral(":/controls/drop_off")).scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        parent
    ) {

    menu = new HoverableMenu(this);
    connect(parent, SIGNAL(hided()), menu, SLOT(close()));
}
DropButton::~DropButton() {}

void DropButton::registerAction(QWidget * action) {
    QWidgetAction * wAction = new QWidgetAction(menu);
    wAction -> setDefaultWidget(action);
    menu -> addAction(wAction);
}

void DropButton::hoverIn() {
    HoverableLabel::hoverIn();

    if (rect().contains(mapFromGlobal(QCursor::pos())))
        if (menu -> isHidden())
            menu -> popup(mapToGlobal(mapFromParent(pos())));
}
