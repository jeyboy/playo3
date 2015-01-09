#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QColor>
#include <QPen>

//    setStyleSheet(QString(
//                      "QTreeView {"
//                          "background-color: black;"
//                      "}"
//                      ));

class Stylesheets {
public:
    static inline QColor color1() { return QColor::fromRgb(181, 189, 200, 212); }
    static inline QColor color2() { return QColor::fromRgb(130, 140, 149, 212); }
    static inline QColor color3() { return QColor::fromRgb(40, 52, 59, 212); }

    static QString appStyles();

    static QString sliderStyles();

    static QString treeViewStyles();

    static QString toolbarStyle();

    static QString toolbarButtonStyle();

    static QString mainWindowTabsStyle();

    static void initBrush(QLinearGradient & brush);

    static void initPens();

    static void calcBorderRect(QRect origin, QRect & res);

    static QPen pen;
    static QPen bevelPen;
    static int borderWidth;
    static int borderRadius;
    static int stickDistance;
};

#endif // STYLESHEETS_H
