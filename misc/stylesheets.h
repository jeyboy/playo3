#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QString>

//    setStyleSheet(QString(
//                      "QTreeView {"
//                          "background-color: black;"
//                      "}"
//                      ));

class Stylesheets {
public:
    static QString appStyles() {
        return QString(
                    "QCheckBox::indicator {"
                    " width: 16px;"
                    " height: 16px;"
                    "}"

                    "QCheckBox::indicator:unchecked {"
                    " image: url(:/elems/check_blank);"
                    "}"

                    "QCheckBox::indicator:unchecked:hover {"
                    " image: url(:/elems/check_blank);"
                    "}"

                    "QCheckBox::indicator:unchecked:disabled {"
                    " image: url(:/elems/check_blank_disabled);"
                    "}"

                    "QCheckBox::indicator:unchecked:pressed {"
                    " image: url(:/elems/check_blank);"
                    "}"

                    "QCheckBox::indicator:checked {"
                    " image: url(:/elems/check_fill);"
                    "}"

                    "QCheckBox::indicator:checked:hover {"
                    " image: url(:/elems/check_trist);"
                    "}"

                    "QCheckBox::indicator:checked:disabled {"
                    " image: url(:/elems/check_fill_disabled);"
                    "}"

                    "QCheckBox::indicator:checked:pressed {"
                    " image: url(:/elems/check_fill);"
                    "}"

                    "QTabBar QToolButton::right-arrow { /* the arrow mark in the tool buttons */"
                    " image: url(:/elems/next);"
                    "}"

                    "QTabBar QToolButton::left-arrow { /* the arrow mark in the tool buttons */"
                    " image: url(:/elems/prev);"
                    "}"

// "QTabBar::tear {"
// " image: url(tear_indicator.png);"
// "}"

                    "QTabBar::scroller {"
                    " height: 24px;"
                    "}"
                );
    }

    static QString sliderStyles() {
        return QString(
                    "QSlider:horizontal {"
                      "margin: 0 4px;"
                    "}"

                    "QSlider:vertical {"
                      "margin: 4px 0;"
                    "}"

                    "QSlider::groove {"
                      "border: 1px solid #bbb;"
                      "border-radius: 2px;"
                    "}"

                    "QSlider::groove:horizontal {"
                      "height: 18px;"
                    "}"

                    "QSlider::groove:vertical {"
                      "width: 18px;"
                    "}"

                    "QSlider::sub-page:horizontal {"
                      "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000, stop: 1 #777);"
                      "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 1, stop: 0 #777, stop: 1 #fff);"
                      "border: 1px solid #777;"
                      "border-radius: 2px;"
                    "}"

                    "QSlider::add-page:horizontal {"
                      "background: #fff;"
                      "border: 1px solid #777;"
                      "border-radius: 2px;"
                    "}"

                    "QSlider::sub-page:vertical {"
                        "background: #fff;"
                        "border: 1px solid #777;"
                        "border-radius: 2px;"
                    "}"

                    "QSlider::add-page:vertical {"
                        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000, stop: 1 #777);"
                        "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 1, stop: 0 #777, stop: 1 #fff);"
                        "border: 1px solid #777;"
                        "border-radius: 2px;"
                    "}"

                    "QSlider::handle {"
                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc);"
                      "border: 1px solid #777;"
                      "margin: 0 -1px;"
                      "border-radius: 2px;"
                    "}"

                    "QSlider::handle:horizontal {"
                      "width: 12px;"
                    "}"

                    "QSlider::handle:vertical {"
                      "height: 12px;"
                    "}"

                    "QSlider::handle:hover {"
                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ddd);"
                      "border: 1px solid #444;"
                      "border-radius: 2px;"
                    "}"

                    "QSlider::sub-page:disabled {"
                      "background: #bbb;"
                      "border-color: #999;"
                    "}"

                    "QSlider::add-page:disabled {"
                      "background: #eee;"
                      "border-color: #999;"
                    "}"

                    "QSlider::handle:disabled {"
                      "background: #eee;"
                      "border: 1px solid #aaa;"
                      "border-radius: 2px;"
                    "}"
        );
    }

    static QString treeViewStyles() {
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

    static QString toolbarButtonStyle() {
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
};

#endif // STYLESHEETS_H
