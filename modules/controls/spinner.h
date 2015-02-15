#ifndef SPINNER
#define SPINNER

#include <qlabel.h>
#include <qstatictext.h>
#include <qpainter.h>
#include <qtimer.h>

class Spinner : public QLabel {
    Q_OBJECT
public:
    Spinner(QString text, int w, int h, QWidget * parent = 0);
    ~Spinner();
public slots:
    void setValue(int percent);
protected slots:
    void continiousProgression();
private:
    void drawSpine(int start, int percent);
    void clearSpine();

    QPainter * img_painter;
    QPixmap * img;
    QStaticText * img_text;
    QRect spine;
    int spineWidth, spinePadd, borderWidth, lastVal, continiousPos;
    QPen * clearPen, * spinePen;

    QTimer timer;
    bool continious;
};

#endif // SPINNER
