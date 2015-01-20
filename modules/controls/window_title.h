#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <qevent.h>
#include "modules/controls/hoverable_label.h"
#include "modules/controls/title_layout.h"

namespace Playo3 { // TODO: add margins param for buttons
    class WindowTitle : public QWidget {
        Q_OBJECT
    public:
        WindowTitle(QWidget * window, int height, QMargins margins, int sidePadding = 0, bool showMini = true, bool showMaxi = true, bool showClose = true);

        void addCustomButton(const QPixmap &icon, const QPixmap &hoverIcon, const QObject * receiver = 0, const char * slot = 0);
        void addMiniButton(const QObject * receiver = 0, const char * slot = 0);
        void addMaxiButton(const QObject * receiver = 0, const char * slot = 0);
        void addCloseButton(const QObject * receiver = 0, const char * slot = 0);

        inline void setText(const QString & text) {
            fullTitle = text;
            int offset = (((QGridLayout *)layout()) -> columnCount()) * (button_height + (((QGridLayout *)layout()) -> horizontalSpacing())); // its little inacurrate
            titleLabel -> setText(titleLabel -> fontMetrics().elidedText(text, Qt::ElideRight, width() - offset));
        }

    public slots:
        inline void invertWindowState() {
            if (parentWidget() -> isMaximized())
                parentWidget() -> showNormal();
            else parentWidget() -> showMaximized();
        }

    signals:
        void doubleClicked();
    protected:
        inline void resizeEvent(QResizeEvent * event) {
            QWidget::resizeEvent(event);
            setText(fullTitle);
        }
        inline void mouseDoubleClickEvent(QMouseEvent *) { emit doubleClicked(); }
        void paintEvent(QPaintEvent *);

    private:
        int button_height;
        QLabel * titleLabel;
        QString fullTitle;
    };
}

#endif // WINDOW_TITLE

