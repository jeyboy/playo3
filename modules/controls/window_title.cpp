#include "window_title.h"

using namespace Playo3;

WindowTitle::WindowTitle(bool compact, QWidget * window, int height, QMargins margins, QMargins buttonsMargins, int left_padding, int right_padding, bool showMini, bool showMaxi, bool showClose)
    : QWidget(window), rightPadding(right_padding), leftPadding(left_padding), isCompact(compact), hMargins(margins) {
    button_height = height - (margins.top() + buttonsMargins.top() + buttonsMargins.bottom());

    dropButton = isCompact ? new DropButton(button_height, this) : 0;

    buttonMargins = buttonsMargins;
    setObjectName("WindowTitle" + QString::number(QDateTime::currentMSecsSinceEpoch()));
    setContentsMargins(hMargins);
    setMinimumHeight(height);

    TitleLayout * l = new TitleLayout(this);

    titleLabel = new RotateLabel("", this);

    if (buttonsMargins.top() != 0 || buttonsMargins.bottom() != 0)
        titleLabel -> setMinimumHeight(height);

    QFont font = titleLabel -> font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel -> setFont(font);

    l -> addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    l -> setColumnStretch(0, 1);

    if (isCompact) {
        dropButton -> setContentsMargins(buttonMargins);

        l -> addWidget(dropButton, 0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
        l -> setColumnStretch(l -> columnCount() - 1, 0);
    }

    connect(this, SIGNAL(doubleClicked()), this, SLOT(invertWindowState()));

    if (showMini)
        addMiniButton();

    if (showMaxi)
        addMaxiButton();

    if (showClose)
        addCloseButton();

    setVertical(false);
}

void WindowTitle::addCustomButton(QString userText, const QPixmap & icon, const QPixmap & hoverIcon, const QObject * receiver, const char * slot) {
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

    if (isCompact)
        dropButton -> registerAction(button);
    else {
        TitleLayout * l = (TitleLayout *)layout();
        l -> addWidget(
            button,
            0, l -> columnCount(), Qt::AlignRight | Qt::AlignVCenter);
        l -> setColumnStretch(l -> columnCount() - 1, 0);
    }
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

void WindowTitle::setVertical(bool isVertical) {
    ((TitleLayout *)layout()) -> setVertical(isVertical);
    titleLabel -> setVertical(isVertical);
    if (isVertical) {
        setStyleSheet("#" + objectName() + " { border-right: 2px solid white; margin: " + QString::number(leftPadding) + "px 0 " + QString::number(rightPadding - 1) + "px 0; }");
        setContentsMargins(5, 0, 0, 0);
    } else {
        setStyleSheet("#" + objectName() + " { border-bottom: 2px solid white; margin: 0 " + QString::number(rightPadding) + "px 0 " + QString::number(leftPadding) + "px; }");
        setContentsMargins(hMargins);
    }
}

void WindowTitle::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
