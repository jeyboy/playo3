#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <qstyleoption.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdatetime.h>
#include <qstringbuilder.h>

#include "modules/core/misc/screen.h"
#include "modules/controls/spinner.h"

#include "hoverable_label.h"
#include "drop_button.h"
#include "title_layout.h"
#include "rotate_label.h"
#include "search_bar.h"

namespace Controls {
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

        void setText(const QString & text);
        inline void setTitleToolTip(QString str) { titleLabel -> setToolTip(str); }
        void setVertical(bool isVertical);
        inline bool isVertical() const { return titleLabel -> isVertical(); }

        void initiateSearch(QWidget * searchContainer, const char * search_start_slot, const char * search_end_slot, const char * search_end_signal);

        inline bool isShowProgress() { return !spinner -> isHidden(); }
        inline void showProgress(bool show) {
            if (show) {
                spinner -> setValue(SPINNER_IS_CONTINIOUS);
                spinner -> show();
            } else
                spinner -> hide();
        }
        inline void setProgress(int progress) { spinner -> setValue(progress); }
    public slots:
        void showSearch();
        void hideSearch();
        void invertWindowState();
    signals:
        void doubleClicked();
        void hided();
    protected:
        inline void hideEvent(QHideEvent * e) {
            emit hided();
            QWidget::hideEvent(e);
        }
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
        SearchBar * search;
        QString fullTitle;
        bool isCompact;
        QMargins hMargins;
        Spinner * spinner;
        QRect baseSize;
    };
}

#endif // WINDOW_TITLE

