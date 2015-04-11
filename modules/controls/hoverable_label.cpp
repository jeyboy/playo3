#include "hoverable_label.h"

using namespace Playo3;


HoverableLabel::HoverableLabel(const QPixmap & icon, const QPixmap & hoverIcon, QWidget * parent, Qt::WindowFlags f, const QObject * receiver, const char * slot)
    : ClickableLabel(icon, parent, f, receiver, slot),  blockEvent(false), ico(QPixmap(icon)), hico(QPixmap(hoverIcon)) {

    connect(this, SIGNAL(hoverIn()), this, SLOT(hoverIn()));
    connect(this, SIGNAL(hoverOut()), this, SLOT(hoverOut()));
}


HoverableLabel::~HoverableLabel() {
}
