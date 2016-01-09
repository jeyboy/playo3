#include "hoverable_label.h"

using namespace Controls;

HoverableLabel::HoverableLabel(QString user_text, const QPixmap & icon, const QPixmap & hoverIcon, QWidget * parent, Qt::WindowFlags f, const QObject * receiver, const char * slot)
    : ClickableLabel(user_text, icon, parent, f, receiver, slot),  blockEvent(false), ico(QPixmap(icon)), hico(QPixmap(hoverIcon)) {

    connect(this, SIGNAL(hoverIn()), this, SLOT(hoverIn()));
    connect(this, SIGNAL(hoverOut()), this, SLOT(hoverOut()));
}

HoverableLabel::~HoverableLabel() {}
