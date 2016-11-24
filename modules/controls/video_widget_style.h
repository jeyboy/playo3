#ifndef VIDEO_WIDGET_STYLE_H
#define VIDEO_WIDGET_STYLE_H

#include <qpainter.h>
#include <qproxystyle.h>

namespace Controls {
    class VideoWidegtStyle : public QProxyStyle {
    public:
        void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = Q_NULLPTR) const {
            painter -> setCompositionMode(QPainter::RasterOp_SourceXorDestination);
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = Q_NULLPTR) const {
            painter -> setCompositionMode(QPainter::RasterOp_SourceXorDestination);
            QProxyStyle::drawControl(element, option, painter, widget);
        }
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = Q_NULLPTR) const {
            painter -> setCompositionMode(QPainter::RasterOp_SourceXorDestination);
            QProxyStyle::drawComplexControl(control, option, painter, widget);
        }
        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const {
            painter -> setCompositionMode(QPainter::RasterOp_SourceXorDestination);
            QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
        }

        void drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const Q_DECL_OVERRIDE {
            painter -> setCompositionMode(QPainter::RasterOp_SourceXorDestination);
            QProxyStyle::drawItemPixmap(painter, rect, alignment, pixmap);
        }

        int styleHint(StyleHint hint, const QStyleOption *option = Q_NULLPTR, const QWidget *widget = Q_NULLPTR, QStyleHintReturn *returnData = Q_NULLPTR) const {
            int u = 0;
            return QProxyStyle::styleHint(hint, option, widget, returnData);
        }
    };
}

#endif // VIDEO_WIDGET_STYLE_H
