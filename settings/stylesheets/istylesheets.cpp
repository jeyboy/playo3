#include "istylesheets.h"
#include "stylesheets_list.h"

IStylesheets::IStylesheets() :resizePen(QPen()), pen(QPen()), bevelPen(QPen()), foregroundPen(QPen()),
    borderWidth(6), borderRadius(12), stickDistance(20) {
    initPens();
    initMainBrush();
    initInnerBrush();
}

IStylesheets * IStylesheets::createStylesheet(const StyleType & stype) {
    switch(stype) {
        case dark: return new DarkStylesheets();
        case light: return new LightStylesheets();
        default: return new IStylesheets();
    }
}

void IStylesheets::applyProperty(QWidget * widget, const char * propertyName, const QVariant & val) {
    widget -> setProperty(propertyName, val);
    widget -> style() -> unpolish(widget);
    widget -> style() -> polish(widget);
    widget -> update();
}

QString IStylesheets::appStyles() {
    return QString(
        "QLabel[timer=\"true\"] {"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"

        "QToolTip {"
        "   border: none;"
        "   background-color: #444;"
        "   color: #ddd; "
//                "   border-radius: 8px;"
//                "   opacity: 300; "
        "}"
    ) % sliderStyles() % treeViewStyles() % toolbarStyles() %
    toolbarButtonStyles() % mainWindowTabsStyles();
}

QString IStylesheets::sliderStyles() {
    return QStringLiteral(
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

        "QSlider::handle[pan=\"true\"] {"
            "image: url(:/controls/pan_btn);"
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

QString IStylesheets::treeViewStyles() {
    return QStringLiteral(
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
    );
}

QString IStylesheets::tokenableStyles(const QColor & textColor) {
    return QString(
        "QWidget[transparent=\"true\"] {"
        "   background-color: transparent;"
        "}"

        "QWidget[borderless=\"true\"] {"
        "   border: none;"
        "}"

        "QWidget[colored_text=\"true\"] {"
        "   color: " % textColor.name(QColor::HexArgb) % ";" // need to change on predefined color
        "}"
    );
}

QString IStylesheets::toolbarStyles() {
    return QStringLiteral(
        "QToolBar[state=\"lighted\"] { border: 2px dotted #00FFFF; }"
    );
}

QString IStylesheets::toolbarButtonStyles() {
    return QStringLiteral(
                "QToolButton[custom=\"true\"] {"
                    "border: 1px solid #444;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #FFF, stop:0.8 #E7DA1E, stop:1 #F7E488);"
// "background: qradialgradient(cx:0.5, cy:0.5, radius: 1, fx:0.2, fy:0.2, stop:0 #FFFFFF, stop:0.5 #B3AF76, stop:1 #F7E488);"
                    "border-radius: 8px;"
                    "font-weight: bold;"
                    "height: 24px;"
                    "min-width: 24px;"
                    "margin: 0 2px;"
                "}"
                "QToolButton[custom=\"true\"]:hover {"
                    "color: #FFF;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #000, stop:0.8 #AAA, stop:1 #666);"
                "}"

                "QToolButton[custom=\"true\"][error=\"true\"] {"
                    "color: #FFF;"
                    "background: qradialgradient(cx:0, cy:0, radius: 1, fx:0.6, fy:0.6, stop:0 #000, stop:1 #AA0000);"
                "}"
              );
}

QString IStylesheets::mainWindowTabsStyles() {
    return QStringLiteral(
                "QMainWindow::separator:hover {"
                "    background: #ff0000;"
                "}"

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
    );
}

QString IStylesheets::downloadProgressStyles() {
    return QStringLiteral(
        "QProgressBar[download] {"
        "   border: 2px solid grey;"
        "   border-radius: 4px;"
        "   text-align: center;"
        "}"

        "QProgressBar[download=\"selected\"] {"
        "   background-color: #999999;"
        "}"

        "QProgressBar::chunk[download] {"
        "   background-color: #05B8CC;"
        /*width: 10px;  margin: 0.5px; border-radius: 6px;*/
        "}"

        "QProgressBar::chunk[download=\"selected\"] {"
        "   background-color: #87CEFA;"
        "}"
    );
}

QString IStylesheets::checkboxStyles() {
    return QStringLiteral(
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

void IStylesheets::calcBorderRect(const QRect & origin, QRect & res) {
    res.setRect(
        origin.x() + borderWidth / 4,
        origin.y() + borderWidth / 4,
        origin.width() - borderWidth / 2,
        origin.height() - borderWidth / 2
    );
}
