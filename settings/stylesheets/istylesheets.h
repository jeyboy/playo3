#ifndef ISTYLESHEETS
#define ISTYLESHEETS

#include <qcolor.h>
#include <qpen.h>
#include <qstringbuilder.h>
#include <qwidget.h>
#include <qstyle.h>

class IStylesheets {
protected:
    QString sliderStyles();
    QString treeViewStyles();
    QString toolbarStyles();
    QString toolbarButtonStyles();
    QString mainWindowTabsStyles();

    QString checkboxStyles();
    QString tokenableStyles(const QColor & textColor = QColor::fromRgb(255, 255, 255));
    QString downloadProgressStyles();
public:
    enum StyleType {
        stub = 0,
        light,
        dark
    };

    static IStylesheets * createStylesheet(const StyleType & stype);

    IStylesheets();
    virtual ~IStylesheets() {}
    virtual StyleType styleType() const { return stub; }

    // background colors
    virtual inline QColor colorResize() { return QColor::fromRgb(0, 255, 255, 64); }

    virtual inline QColor color1() { return QColor::fromRgb(175, 184, 195, 212); }
    virtual inline QColor color2() { return QColor::fromRgb(150, 164, 176, 212); }
    virtual inline QColor color3() { return QColor::fromRgb(130, 140, 149, 212); }

    static void applyProperty(QWidget * widget, const char * propertyName, const QVariant & val);

    virtual QString appStyles();

    virtual void initMainBrush() {}
    virtual void initInnerBrush() {}

    //border pens
    virtual void initPens() {}

    //calc border for custom window
    void calcBorderRect(const QRect & origin, QRect & res);

    // need to rename pen correctly
    QPen resizePen;
    QPen pen;
    QPen foregroundPen;
    QPen bevelPen;

    int borderWidth;
    int borderRadius;
    int stickDistance;

    QLinearGradient mainBrush;
    QLinearGradient innerBrush;
};



#endif // ISTYLESHEETS
