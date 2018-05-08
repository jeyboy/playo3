#include "dark_stylesheets.h"

QString DarkStylesheets::appStyles() {  
    return IStylesheets::appStyles() % QLatin1String(
        "QLabel, QCheckBox, QGroupBox, QRadioButton { color: #EEEEEE; }"
        "QTabWidget QLabel, QTabWidget QCheckBox, QTabWidget QGroupBox, QTabWidget QRadioButton { color: #000000; }"
        "QToolBox QLabel, QToolBox QCheckBox, QToolBox QGroupBox, QToolBox QRadioButton { color: #000000; }"
        "QLabel[darkest=\"true\"] { background-color: #333; border-radius: 6px; padding: 2px; }"
        "*[danger=\"true\"] { background-color: #CCC; border-radius: 6px; padding: 2px; }"
        "QMessageBox QLabel { background-color: #FFF; color: #000; }"
    )   % checkboxStyles() % scrollStyles() % treeViewStyles() % listViewStyles()
        % tokenableStyles() % toolbarStyles() % downloadProgressStyles();
}

QString DarkStylesheets::scrollStyles() {
    return QLatin1String(
        "QScrollBar, QScrollBar::add-line, QScrollBar::sub-line  {"
        "    border: 1px solid grey;"
        "    background: #333;"
        "}"

        "QScrollBar::add-line:hover, QScrollBar::sub-line:hover, QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover  {"
        "    background: #2EA5EA;"
        "}"

        "QScrollBar:horizontal  {"
        "    height: 16px;"
        "    margin: 0px 22px;"
        "}"
        "QScrollBar::handle:horizontal  {"
        "    background: white;"
        "    min-width: 16px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::add-line:horizontal  {"
        "    width: 20px;"
        "    subcontrol-position: right;"
        "    subcontrol-origin: margin;"
        "    border-bottom-right-radius: 8px;"
        "}"

        "QScrollBar::sub-line:horizontal  {"
        "    width: 20px;"
        "    subcontrol-position: left;"
        "    subcontrol-origin: margin;"
        "    border-bottom-left-radius: 8px;"
        "}"

        "QScrollBar:left-arrow:horizontal  {"
        "    background: url(:/controls/scroll_left) no-repeat center center;"
        "}"

        "QScrollBar::right-arrow:horizontal  {"
        "    background: url(:/controls/scroll_right) no-repeat center center;"
        "}"

        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal  {"
        "    background: none;"
        "}"

//// vertical

        "QScrollBar:vertical  {"
        "    width: 16px;"
        "    margin: 22px 0;"
        "}"
        "QScrollBar::handle:vertical  {"
        "    background: white;"
        "    min-height: 16px;"
        "    border-radius: 4px;"
        "}"

        "QScrollBar::add-line:vertical  {"
        "    height: 20px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "    border-bottom-right-radius: 8px;"
        "}"

        "QScrollBar::sub-line:vertical  {"
        "    height: 20px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
//        "    border-top-left-radius: 8px;"
        "    border-top-right-radius: 8px;"
        "}"

        "QScrollBar::up-arrow:vertical  {"
        "    background: url(:/controls/scroll_top) no-repeat center center;"
        "}"

        "QScrollBar::down-arrow:vertical  {"
        "    background: url(:/controls/scroll_bottom) no-repeat center center;"
        "}"

        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical  {"
        "    background: none;"
        "}"
    );
}

QString DarkStylesheets::treeViewStyles() {
    return QString(
        "QTreeView {"
        "   border-radius: 8px;"
        "   background-color: transparent;"
        "}"

        "QTreeView::item, QListView::item {"
        "   color: white;"
        "}"

        "QTreeView::branch::selected {"
            #ifdef HAVE_X11
                "   background-color: " % color3().name(QColor::HexArgb) % ";"
            #endif
        "}"

        "QTreeView::item::selected, QListView::item:selected {"
        "   color: black;"
        "}"
    );
}

QString DarkStylesheets::listViewStyles() {
    return QLatin1String(
        "QListView {"
        "   background-color: transparent;"
        "   border-radius: 8px;"
        "}"
    );
}

QString DarkStylesheets::toolbarStyles() {
    return QLatin1String(
        "QToolBar[state=\"fixed\"] { border: none; }"

        "QToolBar[state=\"float\"] {"
        "   background-color: rgba(0, 0, 0, 48);"
        "   border-radius: 8px;"
        "   border: 1px ridge #888;"
        "}"

        "QToolBar[state=\"movable\"] {"
        "   border-radius: 8px;"
        "   border: 1px ridge #888;"
        "}"
    );
}

void DarkStylesheets::initMainBrush(const QRect & rect) {
    double border = rect.isEmpty() ? .019 : 10 / (double)rect.width();
    QVector<QGradientStop> stops;

    stops << QGradientStop(0,                   hcolor());
    stops << QGradientStop(border,              color1());
    stops << QGradientStop(border + .0001,      color3());
    stops << QGradientStop(.5,                  color2());
    stops << QGradientStop(1 - border - .0001,  color3());
    stops << QGradientStop(1 - border,          color1());
    stops << QGradientStop(1,                   hcolor());

    mainBrush.setStops(stops);
    mainBrush.setStart(rect.topLeft());
    mainBrush.setFinalStop(rect.topRight());
}

void DarkStylesheets::initInnerBrush() {
    innerBrush.setColorAt(0,  QColor(100, 100, 100));
    innerBrush.setColorAt(.5,   QColor(148, 148, 148));
    innerBrush.setColorAt(1,  QColor(100, 100, 100));
}

void DarkStylesheets::initPens() {
    QVector<qreal> penPattern;
    penPattern.append(1); penPattern.append(borderWidth / 2);

    pen.setColor(QColor::fromRgb(255, 255, 255));
    pen.setWidth(borderWidth / 2);
    pen.setCosmetic(true);

    bevelPen.setColor(QColor::fromRgb(23, 23, 23));
    bevelPen.setWidth(borderWidth / 2);
    bevelPen.setCosmetic(true);
    bevelPen.setStyle(Qt::DashLine);
    bevelPen.setDashPattern(penPattern);

    resizePen.setWidth(2);
    resizePen.setColor(colorResize());

    foregroundPen.setColor(QColor::fromRgb(192, 192, 192));
    foregroundPen.setWidth(2);
    foregroundPen.setCosmetic(true);
}
