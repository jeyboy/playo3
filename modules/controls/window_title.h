#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <QGridLayout>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <qevent.h>
#include "modules/controls/hoverable_label.h"

namespace Playo3 {
    class WindowTitle : public QWidget {
        Q_OBJECT
    public:
        WindowTitle(QWidget * window, int height, QMargins margins, int sidePadding = 0, bool showMini = true, bool showMaxi = true, bool showClose = true);

        void addCustomButton(const QPixmap &icon, const QPixmap &hoverIcon, const QObject * receiver = 0, const char * slot = 0);
        void addMiniButton(const QObject * receiver = 0, const char * slot = 0);
        void addMaxiButton(const QObject * receiver = 0, const char * slot = 0);
        void addCloseButton(const QObject * receiver = 0, const char * slot = 0);

        inline void setText(const QString & text) { titleLabel -> setText(text); }
    public slots:
        inline void invertWindowState() {
            if (parentWidget() -> isMaximized())
                parentWidget() -> showNormal();
            else parentWidget() -> showMaximized();
        }

    signals:
        void doubleClicked();
    protected:
        inline void mouseDoubleClickEvent(QMouseEvent *) { emit doubleClicked(); }
        void paintEvent(QPaintEvent *);

    private:
        int button_height;
        QLabel * titleLabel;
    };
}

#endif // WINDOW_TITLE

