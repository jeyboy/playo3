#include "window_title.h"

using namespace Playo3;

WindowTitle::WindowTitle(QWidget * window, int height, QMargins margins, int sidePadding, bool showMini, bool showMaxi, bool showClose) : QWidget(window) {
    button_height = height - margins.top();
    setObjectName("WindowTitle");
    setContentsMargins(margins);
    setStyleSheet("#WindowTitle { border-bottom: 2px solid white; margin: 0 " + QString::number(sidePadding) + "px 0 " + QString::number(sidePadding) + "px; }");
    setFixedHeight(height);
    QGridLayout * l = new QGridLayout(this);
    l -> setContentsMargins(0, 0, 0, 0);

    titleLabel = new QLabel("", this);
    QFont font = titleLabel -> font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel -> setFont(font);

    l -> addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    l -> setColumnStretch(0, 10);

    connect(this, SIGNAL(doubleClicked()), this, SLOT(invertWindowState()));

    if (showMini)
        addMiniButton();
//    {
//        l -> addWidget(
//                    new HoverableLabel(
//                        QPixmap(":mini_button").scaled(height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
//                        QPixmap(":mini_button_hover").scaled(height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
//                        this,
//                        0,
//                        window,
//                        SLOT(showMinimized())
//                    ),
//                    0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
//        l -> setColumnStretch(l -> columnCount() - 1, 0);
//    }

    if (showMaxi)
        addMaxiButton();
//    {
//        l -> addWidget(
//                    new HoverableLabel(
//                        QPixmap(":maxi_button").scaled(height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
//                        QPixmap(":maxi_button_hover").scaled(height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
//                        this,
//                        0,
//                        this,
//                        SLOT(invertWindowState())
//                    ),
//                    0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
//        l -> setColumnStretch(l -> columnCount() - 1, 0);
//    }

    if (showClose)
        addCloseButton();
//    {
//        l -> addWidget(
//                    new HoverableLabel(
//                        QPixmap(":close_button").scaled(height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
//                        QPixmap(":close_button_hover").scaled(height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
//                        this,
//                        0,
//                        window,
//                        SLOT(close())
//                    ),
//                    0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
//        l -> setColumnStretch(l -> columnCount() - 1, 0);
//    }
}

void WindowTitle::addCustomButton(const QPixmap &icon, const QPixmap &hoverIcon, const QObject * receiver, const char * slot) {
    QGridLayout * l = (QGridLayout *)layout();
    l -> addWidget(
                new HoverableLabel(
                    icon.scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
                    hoverIcon.scaled(button_height, button_height, Qt::KeepAspectRatio, Qt::SmoothTransformation),
                    this,
                    0,
                    receiver ? receiver : parent(),
                    slot ? slot : SLOT(showMinimized())
                ),
                0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
    l -> setColumnStretch(l -> columnCount() - 1, 0);
}
void WindowTitle::addMiniButton(const QObject * receiver, const char * slot) {
    addCustomButton(QPixmap(":mini_button"), QPixmap(":mini_button_hover"), receiver, slot ? slot : SLOT(showMinimized()));
}
void WindowTitle::addMaxiButton(const QObject * receiver, const char * slot) {
    addCustomButton(QPixmap(":maxi_button"), QPixmap(":maxi_button_hover"), receiver ? receiver : this, slot ? slot : SLOT(invertWindowState()));
}
void WindowTitle::addCloseButton(const QObject * receiver, const char * slot) {
    addCustomButton(QPixmap(":close_button"), QPixmap(":close_button_hover"), receiver, slot ? slot : SLOT(close()));
}

void WindowTitle::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
