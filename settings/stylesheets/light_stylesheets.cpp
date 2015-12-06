#include "light_stylesheets.h"

QString LightStylesheets::appStyles() {
    return IStylesheets::appStyles()
        % checkboxStyles() % scrollStyles() % treeViewStyles() % listViewStyles()
        % tokenableStyles() % toolbarStyles() % downloadProgressStyles();
}

QString LightStylesheets::scrollStyles() {
    return QStringLiteral(
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

QString LightStylesheets::treeViewStyles() {
    return QString(
        "QTreeView {"
        "   background-color: " + color2().name(QColor::HexArgb) + ";"
        "   border-radius: 8px;"
        "}"
    );
}

QString LightStylesheets::listViewStyles() {
    return QString(
        "QListView {"
        "   background-color: " + color2().name(QColor::HexArgb) + ";"
        "}"
    );
}

QString LightStylesheets::toolbarStyles() {
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

void LightStylesheets::initMainBrush() {
    mainBrush.setColorAt(0,     color1());
    mainBrush.setColorAt(.36,   color2());
    mainBrush.setColorAt(1,     color3());
}
void LightStylesheets::initInnerBrush() {
    innerBrush.setColorAt(0,     color1());
    innerBrush.setColorAt(.36,   color2());
    innerBrush.setColorAt(1,     color3());
}

void LightStylesheets::initPens() {
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
