#include "clickable_label.h"

using namespace Playo3;

ClickableLabel::ClickableLabel(QString user_text, const QString & text, QWidget * parent, Qt::WindowFlags f, const QObject * receiver, const char * slot)
    : QLabel(text, parent, f) {
    setContentsMargins(0,0,0,0);
    if (!user_text.isEmpty())
        setToolTip(user_text);
    if (receiver && slot)
        connect(this, SIGNAL(clicked()), receiver, slot);
}

ClickableLabel::ClickableLabel(QString user_text, const QPixmap & icon, QWidget * parent, Qt::WindowFlags f, const QObject * receiver, const char * slot)
    : QLabel("", parent, f) {
    setPixmap(icon);
    if (!user_text.isEmpty())
        setToolTip(user_text);
    if (receiver && slot)
        connect(this, SIGNAL(clicked()), receiver, slot);
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent *ev) {
    if (ev -> button() == Qt::LeftButton) {
        ev -> accept();
        emit clicked();
    } else
        QLabel::mousePressEvent(ev);
}

bool ClickableLabel::event(QEvent * e) {
    if (e -> type() == QEvent::Resize)
        emit resized();
    if (e -> type() == QEvent::Enter)
        emit hoverIn();
    else if (e -> type() == QEvent::Leave)
        emit hoverOut();

    return QLabel::event(e);
}
