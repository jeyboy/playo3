#include "dark_stylesheets.h"

QString DarkStylesheets::appStyles() {  
    return IStylesheets::appStyles() % QStringLiteral("QLabel, QCheckBox { color: #EEEEEE; }")
        % checkboxStyles() % scrollStyles() % treeViewStyles() % listViewStyles()
        % tokenableStyles() % toolbarStyles() % downloadProgressStyles();
}

QString DarkStylesheets::scrollStyles() {
    return QStringLiteral(
        "[colors] QScrollBar, QScrollBar::add-line, QScrollBar::sub-line  {"
        "    border: 1px solid grey;"
        "    background: #333;"
        "}"

        "[colors] QScrollBar::add-line:hover, QScrollBar::sub-line:hover, QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover  {"
        "    background: #2EA5EA;"
        "}"

        "[colors] QScrollBar:horizontal  {"
        "    height: 16px;"
        "    margin: 0px 22px;"
        "}"
        "[colors] QScrollBar::handle:horizontal  {"
        "    background: white;"
        "    min-width: 16px;"
        "    border-radius: 4px;"
        "}"
        "[colors] QScrollBar::add-line:horizontal  {"
        "    width: 20px;"
        "    subcontrol-position: right;"
        "    subcontrol-origin: margin;"
        "    border-bottom-right-radius: 8px;"
        "}"

        "[colors] QScrollBar::sub-line:horizontal  {"
        "    width: 20px;"
        "    subcontrol-position: left;"
        "    subcontrol-origin: margin;"
        "    border-bottom-left-radius: 8px;"
        "}"

        "[colors] QScrollBar:left-arrow:horizontal  {"
        "    background: url(:/controls/scroll_left) no-repeat center center;"
        "}"

        "[colors] QScrollBar::right-arrow:horizontal  {"
        "    background: url(:/controls/scroll_right) no-repeat center center;"
        "}"

        "[colors] QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal  {"
        "    background: none;"
        "}"

//// vertical

        "[colors] QScrollBar:vertical  {"
        "    width: 16px;"
        "    margin: 22px 0;"
        "}"
        "[colors] QScrollBar::handle:vertical  {"
        "    background: white;"
        "    min-height: 16px;"
        "    border-radius: 4px;"
        "}"

        "[colors] QScrollBar::add-line:vertical  {"
        "    height: 20px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "    border-bottom-right-radius: 8px;"
        "}"

        "[colors] QScrollBar::sub-line:vertical  {"
        "    height: 20px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
//        "    border-top-left-radius: 8px;"
        "    border-top-right-radius: 8px;"
        "}"

        "[colors] QScrollBar::up-arrow:vertical  {"
        "    background: url(:/controls/scroll_top) no-repeat center center;"
        "}"

        "[colors] QScrollBar::down-arrow:vertical  {"
        "    background: url(:/controls/scroll_bottom) no-repeat center center;"
        "}"

        "[colors] QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical  {"
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
    );
}

QString DarkStylesheets::listViewStyles() {
    return QStringLiteral("QListView { background-color: transparent; }");
}

QString DarkStylesheets::toolbarStyles() {
    return QStringLiteral(
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

void DarkStylesheets::initMainBrush() {
    //    mainBrush.setColorAt(0,    QColor(124, 124, 124));
    //    mainBrush.setColorAt(.019,   QColor(80, 80, 80));
    //    mainBrush.setColorAt(.02,  QColor(66, 66, 66));
    //    mainBrush.setColorAt(.2,   QColor(88, 88, 88));
    //    mainBrush.setColorAt(.4,   QColor(55, 55, 55));
    //    mainBrush.setColorAt(.979,  QColor(55, 55, 55));
    //    mainBrush.setColorAt(.98,   QColor(80, 80, 80));
    //    mainBrush.setColorAt(1,    QColor(124, 124, 124));

    //    mainBrush.setColorAt(0,    QColor(168, 168, 168));
    //    mainBrush.setColorAt(.019,   QColor(124, 124, 124));
    //    mainBrush.setColorAt(.02,  QColor(100, 100, 100));
    //    mainBrush.setColorAt(.2,   QColor(132, 132, 132));
    //    mainBrush.setColorAt(.4,   QColor(86, 86, 86));
    //    mainBrush.setColorAt(.979,  QColor(86, 86, 86));
    //    mainBrush.setColorAt(.98,   QColor(124, 124, 124));
    //    mainBrush.setColorAt(1,    QColor(168, 168, 168));

    mainBrush.setColorAt(0,    QColor(168, 168, 168));
    mainBrush.setColorAt(.019,   QColor(124, 124, 124));
    mainBrush.setColorAt(.02,  QColor(100, 100, 100));
    mainBrush.setColorAt(.5,   QColor(148, 148, 148));
    mainBrush.setColorAt(.979,  QColor(100, 100, 100));
    mainBrush.setColorAt(.98,   QColor(124, 124, 124));
    mainBrush.setColorAt(1,    QColor(168, 168, 168));
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
    foregroundPen.setWidth(1);
    foregroundPen.setCosmetic(true);
}
