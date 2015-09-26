#ifndef SPINNER
#define SPINNER

#include <qwidget.h>
#include <qevent.h>
#include <qstatictext.h>
#include <qpainter.h>
#include <qtimer.h>

#define SPINNER_IS_CONTINIOUS -1
#define SPINNER_NOT_SHOW_SECOND -2

namespace Controls {
    class Spinner : public QWidget {
        Q_OBJECT
    public:
        Spinner(QString text, int spinner_width, int spinner_height, QWidget * parent = 0, bool fixed_size = false);
        ~Spinner();
    public slots:
        void setValue(int percent);
        void setValue2(int percent);
        void clear();
        inline void hide() {
            clear();
            QWidget::hide();
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
        bool continious, continious2, show_text;
    };
}

#endif // SPINNER
