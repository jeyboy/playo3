#ifndef SPINNER
#define SPINNER

#include <qlabel.h>
#include <qstatictext.h>
#include <qpainter.h>

class Spinner : QLabel {
    Q_OBJECT
public:
    Spinner(QString text, int w, int h = w, QWidget * parent = 0);
    ~Spinner();
public slots:
    void setValue(int percent);
private:
    QPainter * img_painter;
    QPixmap * img;
    QStaticText * img_text;
    QRect outter, spine, inner;
    int spineWidth, spinePadd;
};

#endif // SPINNER
