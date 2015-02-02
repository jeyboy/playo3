#ifndef TREE_VIEW_STYLE_H
#define TREE_VIEW_STYLE_H

#include <QProxyStyle>
#include <QPainter>

class TreeViewStyle : public QProxyStyle {
public:
    TreeViewStyle() : QProxyStyle() {
        QPalette palette;
        color = palette.link().color();
        color.setAlpha(50);

        pen.setColor(color);
        pen.setWidth(2);

        brush = QBrush(color);
    }

    void drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const {
        if (element == QStyle::PE_IndicatorItemViewItemDrop) {
            painter -> save();
            painter -> setRenderHint(QPainter::Antialiasing, true);

            painter -> setPen(pen);
            painter -> setBrush(brush);

            if(option -> rect.height() == 0) {
                painter -> drawEllipse(option -> rect.topLeft(), 3, 3);
                painter -> drawLine(QPoint(option -> rect.topLeft().x() + 3, option -> rect.topLeft().y()), option -> rect.topRight());
            }
            else {
                int angle = option -> rect.height() / 2.2;
                painter -> drawRoundedRect(option -> rect, angle, angle);
            }

            painter -> restore();
        }
        else QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
private:
    QColor color;
    QPen pen;
    QBrush brush;
};

#endif // TREE_VIEW_STYLE_H
