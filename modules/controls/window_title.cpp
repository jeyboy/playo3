#include "window_title.h"

using namespace Controls;

WindowTitle::WindowTitle(bool compact, QWidget * window, int height, QMargins margins, QMargins buttonsMargins,
            int left_padding, int right_padding, bool showMini, bool showMaxi, bool showClose)
    : QWidget(window), rightPadding(right_padding), leftPadding(left_padding), search(0), isCompact(compact), hMargins(margins)
{
    button_height = height - (margins.top() + buttonsMargins.top() + buttonsMargins.bottom());

    dropButton = isCompact ? new DropButton(button_height, this) : 0;

    buttonMargins = buttonsMargins;
    setObjectName(QLatin1String("WindowTitle") % QString::number(QDateTime::currentMSecsSinceEpoch()));
    setContentsMargins(hMargins);
    setMinimumHeight(height);

    TitleLayout * l = new TitleLayout(this);

    spinner = new Spinner("", 20, 20, this, true);
    l -> addWidget(spinner, 0, 0);
    spinner -> hide();

    titleLabel = new RotateLabel(QString(), this);

    if (buttonsMargins.top() != 0 || buttonsMargins.bottom() != 0)
        titleLabel -> setMinimumHeight(height);

    QFont font = titleLabel -> font();
    font.setPointSize(12);
    font.setBold(true);
    titleLabel -> setFont(font);

    l -> addWidget(titleLabel, 0, 1, Qt::AlignLeft);
    l -> setColumnStretch(1, 1);

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
    addCustomButton(QLatin1String("Minimize"), QPixmap(QLatin1String(":/controls/mini_button")), QPixmap(QLatin1String(":/controls/mini_button_hover")), receiver, slot ? slot : SLOT(showMinimized()));
}
void WindowTitle::addMaxiButton(const QObject * receiver, const char * slot) {
    addCustomButton(QLatin1String("Maximize"), QPixmap(QLatin1String(":/controls/maxi_button")), QPixmap(QLatin1String(":/controls/maxi_button_hover")), receiver ? receiver : this, slot ? slot : SLOT(invertWindowState()));
}
void WindowTitle::addCloseButton(const QObject * receiver, const char * slot) {
    addCustomButton(QLatin1String("Close"), QPixmap(QLatin1String(":/controls/close_button")), QPixmap(QLatin1String(":/controls/close_button_hover")), receiver, slot ? slot : SLOT(close()));
}

void WindowTitle::setText(const QString & text) {
    fullTitle = text;
    int offset;

    if (titleLabel -> isVertical()) {
        offset = (((QGridLayout *)layout()) -> rowCount() - 1) * button_height + 15; // its little inacurrate
        offset = height() - offset;
    } else {
        offset = (((QGridLayout *)layout()) -> columnCount()) * (button_height + (((QGridLayout *)layout()) -> horizontalSpacing())); // its little inacurrate
        offset = width() - offset;
    }

    titleLabel -> setText(titleLabel -> fontMetrics().elidedText(text, Qt::ElideRight, offset));
}

void WindowTitle::setVertical(bool isVertical) {
    ((TitleLayout *)layout()) -> setVertical(isVertical);
    titleLabel -> setVertical(isVertical);
    if (isVertical) {
        setStyleSheet(QLatin1String("#") % objectName() % QLatin1String(" { border-right: 2px solid white; margin: ") % QString::number(leftPadding) % QLatin1String("px 0 ") % QString::number(rightPadding - 1) % QLatin1String("px 0; }"));
        setContentsMargins(5, 0, 0, 0);
    } else {
        setStyleSheet(QLatin1String("#") % objectName() % QLatin1String(" { border-bottom: 2px solid white; margin: 0 ") % QString::number(rightPadding) % QLatin1String("px 0 ") % QString::number(leftPadding) % QLatin1String("px; }"));
        setContentsMargins(hMargins);
    }
}

void WindowTitle::invertWindowState() {
//    QLabel * button = qobject_cast<QLabel *>(sender()); // not worked on double click by title

    if (parentWidget() -> isMaximized()) {
        parentWidget() -> showNormal();
        parentWidget() -> setGeometry(baseSize);
//        button -> setToolTip("Maximize");
    } else {
        QRect area = Screen::screenRect(parentWidget());
        baseSize = parentWidget() -> geometry();
        parentWidget() -> showMaximized();
        if (area.isValid())
            parentWidget() -> setGeometry(area);
//        button -> setToolTip("Normalize");
    }
}

void WindowTitle::initiateSearch(QWidget * searchContainer, const char * search_start_slot, const char * search_end_slot, const char * search_end_signal) {
    if (searchContainer && !search) {
        search = new SearchBar(searchContainer, search_start_slot, search_end_slot, search_end_signal, this, SLOT(hideSearch()), this);
        ((TitleLayout *)layout()) -> addWidget(search, 0, 2);
        search -> hide();
    }
}
void WindowTitle::showSearch() {
    if (search) {
        search -> show();
        search -> setFocus();
    }
}
void WindowTitle::hideSearch() {
    if (search)
        search -> hide();
}

void WindowTitle::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
