#include "button.h"

using namespace Playo3;

Button::Button(const QString & text, const QObject * receiver, const char *slot, QWidget * parent) : QPushButton(text, parent) {
    connect(this, SIGNAL(clicked()), receiver, slot);
    setStyle();
}

Button::Button(const QIcon& icon, const QObject * receiver, const char *slot, QWidget * parent) : QPushButton(icon, "", parent) {
    connect(this, SIGNAL(clicked()), receiver, slot);
    setStyle();
}

Button::Button(const QIcon& icon, const QString &text, const QObject * receiver, const char *slot, QWidget * parent) : QPushButton(icon, text, parent) {
    connect(this, SIGNAL(clicked()), receiver, slot);
    setStyle();
}

void Button::setStyle() {
    setStyleSheet(
                "QPushButton {"
                "    border: 2px solid #8f8f91;"
                "    border-radius: 6px;"
                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                                      stop: 0 #f6f7fa, stop: 1 #dadbde);"
//                "    min-width: 80px;"
                "}"

                "QPushButton:hover {"
                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                                      stop: 0 #dadbde, stop: 1 #f6f7fa);"
                "}"

                "QPushButton:pressed {"
                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                                      stop: 0 #dadbde, stop: 1 #f6f7fa);"
                "}"

                "QPushButton:flat {"
                "    border: none;" /* no border for a flat push button */
                "}"

                "QPushButton:default {"
                "    border-color: navy;" /* make the default button prominent */
                "}"
    );
}
