#ifndef CLICKABLE_LABEL_H
#define CLICKABLE_LABEL_H

#include <qlabel.h>
#include <qevent.h>

namespace Playo3 {
    class ClickableLabel : public QLabel {
      Q_OBJECT

    public:
//        explicit ClickableLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
        explicit ClickableLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0, const QObject * receiver = 0, const char *slot = 0);
        explicit ClickableLabel(const QPixmap &icon, QWidget *parent = 0, Qt::WindowFlags f = 0, const QObject * receiver = 0, const char *slot = 0);
        ~ClickableLabel();

    protected:
        void mousePressEvent(QMouseEvent *ev);
        bool event(QEvent *e);

    signals:
        void clicked();
        void hoverIn();
        void hoverOut();
    };
}

#endif // CLICKABLE_LABEL_H
