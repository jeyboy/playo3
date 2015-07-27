#include "stylesheets.h"

QPen Stylesheets::resizePen = QPen();
QPen Stylesheets::pen = QPen();
QPen Stylesheets::bevelPen = QPen();
int Stylesheets::borderWidth = 6;
int Stylesheets::borderRadius = 12;
int Stylesheets::stickDistance = 20;

QString Stylesheets::appStyles() {
    return QString(
                "QToolTip {"
                "   border: none;"
                "   background-color: #444;"
                "   color: #ddd; "
//                "   border-radius: 8px;"
//                "   opacity: 300; "
                "}"

                "QCheckBox::indicator {"
                "   width: 14px;"
                "   height: 14px;"
                "}"

                "QCheckBox::indicator:unchecked {"
                "   image: url(:/controls/check_blank);"
                "}"

                "QCheckBox::indicator:unchecked:hover {"
                "   image: url(:/controls/check_blank);"
                "}"

                "QCheckBox::indicator:unchecked:disabled {"
                "   image: url(:/controls/check_blank_disabled);"
                "}"

                "QCheckBox::indicator:unchecked:pressed {"
                "   image: url(:/controls/check_blank);"
                "}"

                "QCheckBox::indicator:checked {"
                "   image: url(:/controls/check_fill);"
                "}"

                "QCheckBox::indicator:checked:hover {"
                "   image: url(:/controls/check_tristate);"
                "}"

                "QCheckBox::indicator:checked:disabled {"
                "   image: url(:/controls/check_fill_disabled);"
                "}"

                "QCheckBox::indicator:checked:pressed {"
                "   image: url(:/controls/check_fill);"
                "}"
            );
}

QString Stylesheets::menuStyles() {
    return QString(
//                "QMenu {"
//                  "border-radius: 8px;"
//                "}"
    );
}

QString Stylesheets::sliderStyles() {
    return QString(
                "QSlider:horizontal { margin: 0 4px; }"

                "QSlider:vertical { margin: 4px 0; }"

                "QSlider::add-page, QSlider::sub-page, QSlider::handle, QSlider::groove {"
                    "border: 1px solid #777;"
                    "border-radius: 8px;"
                "}"

                "QSlider::add-page, QSlider::sub-page {"
                    "border-radius: 4px;"
                "}"

                "QSlider::add-page, QSlider::sub-page {"
                    "margin: 4px;"
                    "background: #fff;"
                "}"

                "QSlider::groove {"
                    "border: 1px solid #bbb;"
                "}"

                "QSlider::groove:horizontal {"
                    "height: 20px;"
                "}"

                "QSlider::groove:vertical {"
                    "width: 20px;"
                "}"


//                "QSlider::add-page:horizontal, QSlider::sub-page:vertical {"
//                    "background: #fff;"
//                "}"

                "QSlider::add-page:vertical {"
//                        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000, stop: 1 #777);"
                    "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 1, stop: 0 #777, stop: 1 #fff);"
                "}"

                "QSlider::add-page:disabled {"
                   "background: #eee;"
                   "border-color: #999;"
                "}"

                "QSlider::sub-page:horizontal {"
//                      "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000, stop: 1 #777);"
                  "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 1, stop: 0 #777, stop: 1 #fff);"
                "}"

                "QSlider::sub-page:disabled {"
                    "background: #bbb;"
                    "border-color: #999;"
                "}"

                "QSlider::handle {" // QSlider::handle:horizontal // QSlider::handle:vertical
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc);"
                    "width: 18px;"
                    "height: 18px;"
                "}"

                "QSlider::handle[volume=\"true\"] {"
                    "image: url(:/controls/volume_btn);"
                "}"

                "QSlider::handle[position=\"true\"] {"
                    "image: url(:/controls/pos_btn);"
                "}"

                "QSlider::handle:hover {"
                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ddd);"
                  "border: 1px solid #444;"
                "}"

                "QSlider::handle:disabled {"
                  "background: #eee;"
                  "border: 1px solid #aaa;"
                "}"
    );
}

QString Stylesheets::scrollStyles() {
    return QString(
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

QString Stylesheets::treeViewStyles() {
    return QString(
                "QTreeView {"
                "   background-color: " + color2().name(QColor::HexArgb) + ";"
                "   border-radius: 8px;"
                "}"

                "QTreeView::branch:has-children:!has-siblings:closed,"
                "QTreeView::branch:closed:has-children:has-siblings {"
                "   border-image: none;"
                "   image: url(:/controls/tree_closed);"
                "}"

                "QTreeView::branch:open:has-children:!has-siblings,"
                "QTreeView::branch:open:has-children:has-siblings  {"
                "   border-image: none;"
                "   image: url(:/controls/tree_opened);"
                "}"

               "QTreeView::branch:has-children:!has-siblings:closed:hover,"
               "QTreeView::branch:closed:has-children:has-siblings:hover {"
               "   border-image: none;"
               "   image: url(:/controls/tree_closed_hover);"
               "}"

               "QTreeView::branch:open:has-children:!has-siblings:hover,"
               "QTreeView::branch:open:has-children:has-siblings:hover  {"
               "   border-image: none;"
               "   image: url(:/controls/tree_opened_hover);"
               "}"
              ) + scrollStyles();
}

QString Stylesheets::listViewStyles() {
    return QString(
                "QListView {"
                "   background-color: " + color2().name(QColor::HexArgb) + ";"
                "   border-bottom-left-radius: 8px;"
                "   border-bottom-right-radius: 8px;"
                "}"
              );
}

QString Stylesheets::toolbarHighLightStyle() {
    return QString("QToolBar { border: 2px dotted #00FFFF; }");
}

QString Stylesheets::toolbarFixedStyle() {
    return QString("QToolBar { border: none; }");
}

QString Stylesheets::toolbarFloatStyle() {
    return QString(
                "QToolBar {"
                "   background-color: rgba(0, 0, 0, 48);"
                "   border-radius: 8px;"
                "   border: 1px ridge #888;"
                "}"
    );
}

QString Stylesheets::toolbarMovableStyle() {
    return QString(
                "QToolBar {"
                "   border-radius: 8px;"
                "   border: 1px ridge #888;"
                "}"
    );
}

QString Stylesheets::toolbarButtonStyle() {
    return QString(
                "QToolButton {"
                    "border: 1px solid #444;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #FFF, stop:0.8 #E7DA1E, stop:1 #F7E488);"
// "background: qradialgradient(cx:0.5, cy:0.5, radius: 1, fx:0.2, fy:0.2, stop:0 #FFFFFF, stop:0.5 #B3AF76, stop:1 #F7E488);"
                    "border-radius: 8px;"
                    "font-weight: bold;"
                    "height: 24px;"
                    "min-width: 24px;"
                    "margin: 0 2px;"
                "}"
                "QToolButton:hover {"
                    "color: #FFF;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #000, stop:0.8 #AAA, stop:1 #666);"
                "}"

                "QToolButton[error=\"true\"] {"
                    "color: #FFF;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #000, stop:1 #AA0000);"
                "}"
              );
}

QString Stylesheets::mainWindowTabsStyle() {
    return QString(
                "QMainWindow::separator:hover {"
                "    background: #ff0000;"
                "}"

////                "QMainWindow::separator {"
////                "   background: red;"
////                "}"

                "QTabBar::scroller {" /* the width of the scroll buttons */
                "    width: 20px;"
                "}"

//                    "QTabBar QToolButton {" /* the scroll buttons are tool buttons */
//                    "    border-image: url(scrollbutton.png) 2;"
//                    "    border-width: 2px;"
//                    "}"

//                "QTabBar QToolButton::right-arrow {" /* the arrow mark in the tool buttons */
//                "    image: url(rightarrow.png);"
//                "}"

//                "QTabBar QToolButton::left-arrow {"
//                "    image: url(leftarrow.png);"
//                "}"

                "QTabBar {"
                "   alignment: center;"
                "}"

                "QTabBar::tab:top, QTabBar::tab:bottom {"
                "   height: 20px;"
                "}"

                "QTabBar::tab:left, QTabBar::tab:right {"
                "   width: 20px;"
                "}"
    ) + treeViewStyles() + listViewStyles() + menuStyles();
}

void Stylesheets::initBrush(QLinearGradient & brush) {
    brush.setColorAt(0,     color1());
    brush.setColorAt(.36,   color2());
    brush.setColorAt(1,     color3());
}

void Stylesheets::initPens() {
    QVector<qreal> penPattern;
    penPattern.append(1); penPattern.append(borderWidth / 2);

    pen.setColor(QColor::fromRgb(255, 255, 255));
    pen.setWidth(borderWidth / 2);
//    pen.setCosmetic(true);

    bevelPen.setColor(QColor::fromRgb(23, 23, 23));
    bevelPen.setWidth(borderWidth / 2);
//    bevelPen.setCosmetic(true);
    bevelPen.setStyle(Qt::DashLine);
    bevelPen.setDashPattern(penPattern);

    resizePen.setWidth(2);
    resizePen.setColor(colorResize());
}

void Stylesheets::calcBorderRect(QRect origin, QRect & res) {
    res.setRect(
        origin.x() + borderWidth / 4,
        origin.y() + borderWidth / 4,
        origin.width() - borderWidth / 2,
        origin.height() - borderWidth / 2
    );
}
