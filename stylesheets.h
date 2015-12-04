#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <qcolor.h>
#include <qpen.h>
#include <qstringbuilder.h>
#include <qwidget.h>
#include <qstyle.h>

class Stylesheets {
public:
    enum StyleType {
        usuall = 0,
        light,
        dark
    };

    // background colors
    static inline QColor colorResize() { return QColor::fromRgb(0, 255, 255, 64); }

    static inline QColor color1() { return QColor::fromRgb(175, 184, 195, 212); }
    static inline QColor color2() { return QColor::fromRgb(150, 164, 176, 212); }
    static inline QColor color3() { return QColor::fromRgb(130, 140, 149, 212); }

    static void applyProperty(QWidget * widget, const char * propertyName, const QVariant & val) {
        widget -> setProperty(propertyName, val);
        widget -> style() -> unpolish(widget);
        widget -> style() -> polish(widget);
        widget -> update();
    }

    static QString appStyles();

    static QString menuStyles();

    static QString sliderStyles();

    static QString treeViewStyles();
    static QString listViewStyles();

    static QString tokenableStyles();

    static QString scrollStyles();

    static QString toolbarStyles();

    static QString toolbarButtonStyle();

    static QString mainWindowTabsStyle();

    static QString downloadProgressStyles();

    //background brush
    static void initBrush(QLinearGradient & brush);

    static void initInnerBrush(QLinearGradient & brush);

    //border pens
    static void initPens();

    //calc border for custom window
    static void calcBorderRect(const QRect & origin, QRect & res);

    static QPen resizePen;
    static QPen pen;
    static QPen foregroundPen;
    static QPen bevelPen;
    static int borderWidth;
    static int borderRadius;
    static int stickDistance;
};

#endif // STYLESHEETS_H
