#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <QGridLayout>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <qevent.h>

class WindowTitle : public QWidget {
    Q_OBJECT
public:
    inline WindowTitle(QWidget * window, int height, QMargins margins, int sidePadding) : QWidget(window) {
        setObjectName("WindowTitle");
        setContentsMargins(margins);
        setStyleSheet("#WindowTitle { border-bottom: 2px solid white; margin: 0 " + QString::number(sidePadding) + "px 0 " + QString::number(sidePadding) + "px; }");
        setFixedHeight(height);
        (new QGridLayout(this)) -> setContentsMargins(0, 0, 0, 0);
    }

signals:
    void doubleClicked();
protected:
    inline void mouseDoubleClickEvent(QMouseEvent *) { emit doubleClicked(); }
    inline void paintEvent(QPaintEvent *) {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // WINDOW_TITLE

