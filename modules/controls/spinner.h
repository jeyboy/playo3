#ifndef SPINNER
#define SPINNER

#include <qwidget.h>
#include <qevent.h>
#include <qstatictext.h>
#include <qpainter.h>
#include <qtimer.h>

class Spinner : public QWidget {
    Q_OBJECT
public:
    Spinner(QString text, int w, int h, QWidget * parent = 0);
    ~Spinner();
public slots:
    void setValue(int percent);
protected slots:
    void continiousProgression();
protected:
    void paintEvent(QPaintEvent *);
private:
    void drawSpine(int start, int percent);
    void clearSpine();

    QPainter * img_painter;
    QPixmap * img;
    QStaticText * img_text;
    QRect spine;
    int spineWidth, spinePad, borderWidth, lastVal, continiousPos;
    QPen * clearPen, * spinePen;

    QTimer timer;
    bool continious;
};

#endif // SPINNER
