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

//                "QCheckBox::indicator {"
//                " width: 16px;"
//                " height: 16px;"
//                "}"

//                "QCheckBox::indicator:unchecked {"
//                " image: url(:/elems/check_blank);"
//                "}"

//                "QCheckBox::indicator:unchecked:hover {"
//                " image: url(:/elems/check_blank);"
//                "}"

//                "QCheckBox::indicator:unchecked:disabled {"
//                " image: url(:/elems/check_blank_disabled);"
//                "}"

//                "QCheckBox::indicator:unchecked:pressed {"
//                " image: url(:/elems/check_blank);"
//                "}"

//                "QCheckBox::indicator:checked {"
//                " image: url(:/elems/check_fill);"
//                "}"

//                "QCheckBox::indicator:checked:hover {"
//                " image: url(:/elems/check_trist);"
//                "}"

//                "QCheckBox::indicator:checked:disabled {"
//                " image: url(:/elems/check_fill_disabled);"
//                "}"

//                "QCheckBox::indicator:checked:pressed {"
//                " image: url(:/elems/check_fill);"
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

QString Stylesheets::treeViewStyles() { // NOT USED
    return QString(
                "background-color: black;"
                "QTreeView::indicator {"
                "   width: 18px;"
                "   height: 18px;"
                "}"
                "QTreeView::indicator:unchecked {"
                "   image: url(:/elems/check_blank);"
                "}"
                "QTreeView::indicator:checked {"
                "   image: url(:/elems/check_fill);"
                "}"
                "QTreeView::indicator:checked:hover {"
                "   image: url(:/elems/check_trist);"
                "}"
                "QTreeView::indicator:indeterminate:hover {"
                "   image: url(:/elems/check_trist);"
                "}"
                "QTreeView::indicator:indeterminate {"
                "   image: url(:/elems/check_fill);"
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
              );
}

QString Stylesheets::mainWindowTabsStyle() {
    return QString(
                "QMainWindow::separator:hover {"
                "    background: #ff0000;"
                "}"

//                "QMainWindow::separator {"
//                "   background: red;"
//                "}"

//                "QToolBar::handle {"
//                "    margin-top: 20px;"
//                "}"

                "QTabBar::tab {"
                "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                               stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
                "                               stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
                "   border: 2px solid #C4C4C3;"
                "   border-bottom-color: #C2C7CB;"
                "}"

                "QTabBar::tab:top {"
                "   border-top-left-radius: 4px;"
                "   border-top-right-radius: 4px;"
                "   min-width: 60px;"
                "}"

                "QTabBar::tab:bottom {"
                "   border-bottom-left-radius: 4px;"
                "   border-bottom-right-radius: 4px;"
                "   min-width: 60px;"
                "}"

                "QTabBar::tab:left {"
                "   border-top-left-radius: 4px;"
                "   border-bottom-left-radius: 4px;"
                "   min-height: 60px;"
                "}"

                "QTabBar::tab:right {"
                "   border-top-right-radius: 4px;"
                "   border-bottom-right-radius: 4px;"
                "   min-height: 60px;"
                "}"

                "QTabBar::tab:selected, QTabBar::tab:hover {"
                "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                               stop: 0 #fafafa, stop: 0.4 #f4f4f4,"
                "                               stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
                "}"

                "QTabBar::tab:selected {"
                "   border-color: #9B9B9B;"
                "   border-bottom-color: #C2C7CB;" /* same as pane color */
                "}"

                "QTabBar::tab:!selected {"
                "   margin-top: 2px;" /* make non-selected tabs look smaller */
                "}"

                /* make use of negative margins for overlapping tabs */
                "QTabBar::tab:selected {"
                "   margin-left: -4px;"
                "   margin-right: -4px;"
                "}"

                "QTabBar::tab:first:selected {"
                "   margin-left: 0;"
                "}"

                "QTabBar::tab:last:selected {"
                "   margin-right: 0;"
                "}"

                "QTabBar::tab:only-one {"
                "   margin: 0;"
                "}"

//                    QTabBar::tear {
//                        image: url(tear_indicator.png);
//                    }

                "QTabBar::scroller {" /* the width of the scroll buttons */
                "    width: 20px;"
                "}"

//                    "QTabBar QToolButton {" /* the scroll buttons are tool buttons */
//                    "    border-image: url(scrollbutton.png) 2;"
//                    "    border-width: 2px;"
//                    "}"

//                    "QTabBar QToolButton::right-arrow {" /* the arrow mark in the tool buttons */
//                    "    image: url(rightarrow.png);"
//                    "}"

//                    "QTabBar QToolButton::left-arrow {"
//                    "    image: url(leftarrow.png);"
//                    "}"
    );
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
