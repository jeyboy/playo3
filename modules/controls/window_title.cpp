#include "window_title.h"

using namespace Playo3;

WindowTitle::WindowTitle(QWidget * window, int height, QMargins margins, QMargins buttonsMargins, int leftPadding, int rightPadding, bool showMini, bool showMaxi, bool showClose)
    : QWidget(window) {
    button_height = height - (margins.top() + buttonsMargins.top() + buttonsMargins.bottom());
    buttonMargins = buttonsMargins;
    setObjectName("WindowTitle");
    setContentsMargins(margins);
    setMinimumHeight(height);
    setStyleSheet("#WindowTitle { border-bottom: 2px solid white; margin: 0 " + QString::number(rightPadding) + "px 0 " + QString::number(leftPadding) + "px; }");
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    TitleLayout * l = new TitleLayout(this);

    titleLabel = new QLabel("", this);

    if (buttonsMargins.top() != 0 || buttonsMargins.bottom() != 0)
        titleLabel -> setMinimumHeight(height);

    QFont font = titleLabel -> font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel -> setFont(font);

    l -> addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    l -> setColumnStretch(0, 1);

    connect(this, SIGNAL(doubleClicked()), this, SLOT(invertWindowState()));

    if (showMini)
        addMiniButton();

    if (showMaxi)
        addMaxiButton();

    if (showClose)
        addCloseButton();
}

void WindowTitle::addCustomButton(QString userText, const QPixmap & icon, const QPixmap & hoverIcon, const QObject * receiver, const char * slot) {
    QGridLayout * l = (QGridLayout *)layout();

    HoverableLabel * button =
            new HoverableLabel(
                userText,
                icon.scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
                hoverIcon.scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
                this,
                0,
                receiver ? receiver : parent(),
                slot ? slot : SLOT(showMinimized())
            );

    button -> setContentsMargins(buttonMargins);

    l -> addWidget(
                button,
                0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
    l -> setColumnStretch(l -> columnCount() - 1, 0);
}
void WindowTitle::addMiniButton(const QObject * receiver, const char * slot) {
    addCustomButton("Minimize", QPixmap(":mini_button"), QPixmap(":mini_button_hover"), receiver, slot ? slot : SLOT(showMinimized()));
}
void WindowTitle::addMaxiButton(const QObject * receiver, const char * slot) {
    addCustomButton("Maximize", QPixmap(":maxi_button"), QPixmap(":maxi_button_hover"), receiver ? receiver : this, slot ? slot : SLOT(invertWindowState()));
}
void WindowTitle::addCloseButton(const QObject * receiver, const char * slot) {
    addCustomButton("Close", QPixmap(":close_button"), QPixmap(":close_button_hover"), receiver, slot ? slot : SLOT(close()));
}

void WindowTitle::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
