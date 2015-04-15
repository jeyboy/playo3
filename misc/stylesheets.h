#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QColor>
#include <QPen>

class Stylesheets {
public:
    // background colors
    static inline QColor colorResize() { return QColor::fromRgb(0, 255, 255, 64); }

    static inline QColor color1() { return QColor::fromRgb(175, 184, 195, 212); }
    static inline QColor color2() { return QColor::fromRgb(150, 164, 176, 212); }
    static inline QColor color3() { return QColor::fromRgb(130, 140, 149, 212); }
//    static inline QColor color3() { return QColor::fromRgb(40, 52, 59, 212); }

    static QString appStyles();

    static QString menuStyles();

    static QString sliderStyles();

    static QString treeViewStyles();
    static QString listViewStyles();

    static QString scrollStyles();

    static QString toolbarHighLightStyle();
    static QString toolbarFixedStyle();
    static QString toolbarFloatStyle();
    static QString toolbarMovableStyle();

    static QString toolbarButtonStyle();

    static QString mainWindowTabsStyle();

    //background brush
    static void initBrush(QLinearGradient & brush);

    //border pens
    static void initPens();

    //calc border for custom window
    static void calcBorderRect(QRect origin, QRect & res);

    static QPen resizePen;
    static QPen pen;
    static QPen bevelPen;
    static int borderWidth;
    static int borderRadius;
    static int stickDistance;
};

#endif // STYLESHEETS_H
