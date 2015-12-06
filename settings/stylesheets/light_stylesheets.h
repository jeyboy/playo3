#ifndef LIGHT_STYLESHEETS_H
#define LIGHT_STYLESHEETS_H

#include "istylesheets.h"

class LightStylesheets : public IStylesheets {
    QString treeViewStyles();
    QString listViewStyles();
    QString scrollStyles();
    QString toolbarStyles();
public:
    LightStylesheets() {
        initPens();
        initMainBrush();
        initInnerBrush();
    }

    StyleType styleType() const { return light; }
    virtual bool isTransparent() { return true; }

    // background colors
    inline QColor colorResize() { return QColor::fromRgb(0, 255, 255, 64); }

    inline QColor color1() { return QColor::fromRgb(175, 184, 195, 212); }
    inline QColor color2() { return QColor::fromRgb(150, 164, 176, 212); }
    inline QColor color3() { return QColor::fromRgb(130, 140, 149, 212); }

    QString appStyles();

    void initMainBrush(const QRect & /*rect*/ = QRect());
    void initInnerBrush();

    void initPens();
};

#endif // LIGHT_STYLESHEETS_H
