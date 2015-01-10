#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <qevent.h>
#include "modules/controls/hoverable_label.h"
#include "modules/controls/title_layout.h"
#include <qdebug.h>

namespace Playo3 {
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
            titleLabel -> setText(titleLabel -> fontMetrics().elidedText(text, Qt::ElideRight, width() - offset, Qt::TextWrapAnywhere));
        }
        inline bool inAction() const { return in_action; }

    public slots:
        inline void invertWindowState() {
            if (parentWidget() -> isMaximized())
                parentWidget() -> showNormal();
            else parentWidget() -> showMaximized();
        }

    signals:
        void pressed();
//        void released();
        void doubleClicked();
    protected:
        inline void resizeEvent(QResizeEvent * event) {
            setText(fullTitle);
            QWidget::resizeEvent(event);
        }
        inline void mouseDoubleClickEvent(QMouseEvent *) { emit doubleClicked(); }
        inline void mousePressEvent(QMouseEvent * e) {
            if ((in_action = e -> button() == Qt::LeftButton))
                emit pressed();
            QWidget::mousePressEvent(e);
        }
//        inline void mouseReleaseEvent(QMouseEvent * e) {
//            qDebug() << "RE " << in_action;
//            if (in_action)
//                emit released();
//            in_action = false;
//            QWidget::mouseReleaseEvent(e);
//        }
        void paintEvent(QPaintEvent *);

    private:
        bool in_action;
        int button_height;
        QLabel * titleLabel;
        QString fullTitle;
    };
}

#endif // WINDOW_TITLE

