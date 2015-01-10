#include "window_title.h"

using namespace Playo3;

WindowTitle::WindowTitle(QWidget * window, int height, QMargins margins, int sidePadding, bool showMini, bool showMaxi, bool showClose)
    : QWidget(window), in_action(false) {
    button_height = height - margins.top();
    setObjectName("WindowTitle");
    setContentsMargins(margins);
    setStyleSheet("#WindowTitle { border-bottom: 2px solid white; margin: 0 " + QString::number(sidePadding) + "px 0 " + QString::number(sidePadding) + "px; }");
    setFixedHeight(height);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    TitleLayout * l = new TitleLayout(this);

    titleLabel = new QLabel("", this);
    titleLabel -> setWordWrap(false);
    titleLabel -> setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QFont font = titleLabel -> font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel -> setFont(font);

    l -> addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    l -> setColumnStretch(0, 10);

    connect(this, SIGNAL(doubleClicked()), this, SLOT(invertWindowState()));

    if (showMini)
        addMiniButton();

    if (showMaxi)
        addMaxiButton();

    if (showClose)
        addCloseButton();
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
