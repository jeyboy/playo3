#ifndef SPINNER
#define SPINNER

#include <qwidget.h>
#include <qevent.h>
#include <qstatictext.h>
#include <qtimer.h>
#include <qimage.h>

#define SPINNER_IS_CONTINIOUS -1
#define SPINNER_NOT_SHOW_SECOND -2

namespace Controls {
    class Spinner : public QWidget {
        Q_OBJECT
    public:
        Spinner(const QString & text, int spinner_width, int spinner_height, QWidget * parent = 0, bool fixed_size = false, const QImage & def_ico = QImage());
        ~Spinner();
    public slots:
        void setDefaultIcon(const QImage & new_def_ico) {
            def_ico = new_def_ico;
            if (isHidden()) { // rehide image for appling of new state
                show();
                hide();
            }
        }

        void setValue(int percent);
        void setValue2(int percent);
        void clear();

        inline bool isHidden() const {
            return show_def_ico || QWidget::isHidden();
        }

        inline void show() {
            if (show_def_ico) {
                show_def_ico = false;
                repaint();
            }

            QWidget::show();
        }
        inline void hide() {
            clear();
            if (!(show_def_ico = !def_ico.isNull()))
                QWidget::hide();
            else
                repaint();
        }
    protected slots:
        void continiousProgression();
        void continiousProgression2();
    protected:
        void paintEvent(QPaintEvent *);
        void resizeEvent(QResizeEvent *);
    private:
        QStaticText * img_text;
        QRect spine, inner_spine, outter, inner, ininner;
        int spineWidth, spinePad, borderWidth, continiousLen, w, h;
        int lastVal, lastVal2;
        int continiousPos, continiousPos2;
        QPoint textPoint;
        QPen * clearPen, * spinePen, * borderPen;

        QTimer timer, timer2;
        bool continious, continious2, show_text, show_def_ico;
        QImage def_ico;
    };
}

#endif // SPINNER
