#include "clickable_label.h"

using namespace Playo3;

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f) {

}
ClickableLabel::ClickableLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) : QLabel(text, parent, f) {

}

ClickableLabel::ClickableLabel(const QPixmap &icon, const QObject * receiver, const char *slot, QWidget *parent, Qt::WindowFlags f) : QLabel("", parent, f) {
    setPixmap(icon.scaled(width(), 20, Qt::KeepAspectRatio));
    connect(this, SIGNAL(clicked()), receiver, slot);
}

ClickableLabel::~ClickableLabel() {

}

void ClickableLabel::mousePressEvent(QMouseEvent *ev) {
    if (ev -> button() == Qt::LeftButton)
        emit clicked();

    QLabel::mousePressEvent(ev);
}
