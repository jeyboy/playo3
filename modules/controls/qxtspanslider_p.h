#ifndef QXTSPANSLIDER_P_H
#define QXTSPANSLIDER_P_H

#include <qstyle.h>
#include "qxtspanslider.h"

QT_FORWARD_DECLARE_CLASS(QStylePainter)
QT_FORWARD_DECLARE_CLASS(QStyleOptionSlider)

class QxtSpanSliderPrivate : public QObject, public QxtPrivate<QxtSpanSlider> {
    Q_OBJECT
public:
    QXT_DECLARE_PUBLIC(QxtSpanSlider)

    QxtSpanSliderPrivate();
    void initStyleOption(QStyleOptionSlider * option, QxtSpanSlider::SpanHandle handle = QxtSpanSlider::UpperHandle) const;
    int pick(const QPoint & pt) const { return qxt_p().orientation() == Qt::Horizontal ? pt.x() : pt.y(); }
    int pixelPosToRangeValue(int pos) const;
    void handleMousePress(const QPoint & pos, QStyle::SubControl& control, int value, QxtSpanSlider::SpanHandle handle);
    void drawHandle(QStylePainter * painter, QxtSpanSlider::SpanHandle handle) const;
    void setupPainter(QPainter * painter, Qt::Orientation orientation, qreal x1, qreal y1, qreal x2, qreal y2) const;
    void drawSpan(QStylePainter * painter, const QRect & rect) const;
    void triggerAction(QAbstractSlider::SliderAction action, bool main);
    void swapControls();

    int lower;
    int upper;
    int lowerPos;
    int upperPos;
    int offset;
    int position;
    QxtSpanSlider::SpanHandle lastPressed;
    QxtSpanSlider::SpanHandle mainControl;
    QStyle::SubControl lowerPressed;
    QStyle::SubControl upperPressed;
    QxtSpanSlider::HandleMovementMode movement;
    bool firstMovement;
    bool blockTracking;
    bool showRail;

public Q_SLOTS:
    void updateRange(int min, int max);
    void movePressedHandle();
};

#endif // QXTSPANSLIDER_P_H
