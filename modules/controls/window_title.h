#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <qevent.h>
#include "hoverable_label.h"
#include "drop_button.h"
#include "title_layout.h"
#include "rotate_label.h"

namespace Playo3 {
    class WindowTitle : public QWidget {
        Q_OBJECT
    public:
        WindowTitle(bool compact, QWidget * window, int height = 30, QMargins margins = QMargins(),
                    QMargins buttonsMargins = QMargins(), int leftPadding = 0, int rightPadding = 0,
                    bool showMini = true, bool showMaxi = true, bool showClose = true);

        void addCustomButton(QString userText, const QPixmap & icon, const QPixmap & hoverIcon, const QObject * receiver = 0, const char * slot = 0);
        void addMiniButton(const QObject * receiver = 0, const char * slot = 0);
        void addMaxiButton(const QObject * receiver = 0, const char * slot = 0);
        void addCloseButton(const QObject * receiver = 0, const char * slot = 0);

        inline void setText(const QString & text) {
            fullTitle = text;

            if (titleLabel -> isVertical()) {
                int offset = (((QGridLayout *)layout()) -> rowCount()) * (button_height + (((QGridLayout *)layout()) -> verticalSpacing())); // its little inacurrate
                titleLabel -> setText(titleLabel -> fontMetrics().elidedText(text, Qt::ElideRight, height() - offset));
            } else {
                int offset = (((QGridLayout *)layout()) -> columnCount()) * (button_height + (((QGridLayout *)layout()) -> horizontalSpacing())); // its little inacurrate
                titleLabel -> setText(titleLabel -> fontMetrics().elidedText(text, Qt::ElideRight, width() - offset));
            }
        }
        void setVertical(bool isVertical);

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
        int button_height, rightPadding, leftPadding;
        QMargins buttonMargins;
        RotateLabel * titleLabel;
        DropButton * dropButton;
        QString fullTitle;
        bool isCompact;
    };
}

#endif // WINDOW_TITLE

