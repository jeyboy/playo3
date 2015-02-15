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
    void resizeEvent(QResizeEvent *);
private:
    void drawSpine(int start, int percent);
    void clearSpine();

    QStaticText * img_text;
    QRect spine, outter, inner;
    int spineWidth, spinePad, borderWidth, lastVal, continiousPos, continiousLen;
    QPoint textPoint;
    QPen * clearPen, * spinePen, * borderPen;

    QTimer timer;
    bool continious;
};

#endif // SPINNER
