#ifndef ISTYLESHEETS
#define ISTYLESHEETS

#include <qcolor.h>
#include <qpen.h>
#include <qstringbuilder.h>
#include <qwidget.h>
#include <qstyle.h>

class IStylesheets {
protected:
    QString _sliderStyles();
    QString _treeViewStyles();
    QString _toolbarStyles();
    QString _toolbarButtonStyles();
    QString _mainWindowTabsStyles();

    QString checkboxStyles();
    QString tokenableStyles(const QColor & textColor = QColor::fromRgb(255, 255, 255));
    QString downloadProgressStyles();
public:
    enum StyleType : int {
        stub = 0,
        light,
        dark
    };

    static IStylesheets * createStylesheet(const StyleType & stype);

    IStylesheets();
    virtual ~IStylesheets() {}
    virtual StyleType styleType() const { return stub; }
    virtual bool isTransparent() { return false; }

    // background colors
    virtual inline QColor colorResize() { return QColor::fromRgb(0, 255, 255, 64); }

    virtual inline QColor color1() { return QColor::fromRgb(255, 255, 255, 255); }
    virtual inline QColor color2() { return QColor::fromRgb(255, 255, 255, 255); }
    virtual inline QColor color3() { return QColor::fromRgb(255, 255, 255, 255); }

    static void applyProperty(QWidget * widget, const char * propertyName, const QVariant & val);

    virtual QString appStyles();

    virtual void initMainBrush(const QRect & /*rect*/ = QRect()) {}
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
