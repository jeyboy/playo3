#ifndef DARK_STYLESHEETS_H
#define DARK_STYLESHEETS_H

#include "istylesheets.h"

class DarkStylesheets : public IStylesheets {
    QString treeViewStyles();
    QString listViewStyles();
    QString scrollStyles();
    QString toolbarStyles();
public:
    DarkStylesheets() {
        initPens();
        initMainBrush();
        initInnerBrush();
    }

    StyleType styleType() const { return dark; }

    QImage loadableTabIcon() { return QImage(QLatin1String(":/download_on")); }
    virtual QImage background() {
        QImage img = IStylesheets::background();
        img.invertPixels();
        return img;
    }

    inline QColor colorResize() { return QColor::fromRgb(0, 255, 255, 64); }

    inline QColor hcolor() { return QColor::fromRgb(168, 168, 168, 232); }
    inline QColor color1() { return QColor(124, 124, 124); }
    inline QColor color2() { return QColor(148, 148, 148); }
    inline QColor color3() { return QColor(100, 100, 100); }

    QString appStyles();

    void initMainBrush(const QRect & /*rect*/ = QRect());
    void initInnerBrush();
    void initPens();
};

#endif // DARK_STYLESHEETS_H
