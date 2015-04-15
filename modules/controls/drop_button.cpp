#include "drop_button.h"

using namespace Playo3;

DropButton::DropButton(int button_height, QWidget * parent) :
    HoverableLabel(
        "",
        QPixmap(":drop_off").scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        QPixmap(":drop_on").scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        parent
    ) {

    menu = new HoverableMenu(this);
    menu -> setTearOffEnabled(false);
}
DropButton::~DropButton() {}

void DropButton::registerAction(QWidget * action) {
    QWidgetAction * wAction = new QWidgetAction(menu);
    wAction -> setDefaultWidget(action);
    menu -> addAction(wAction);
}

void DropButton::hoverIn() {
    HoverableLabel::hoverIn();
    menu -> popup(mapToGlobal(mapFromParent(pos())));
}
