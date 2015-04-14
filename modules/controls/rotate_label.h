#ifndef ROTATE_LABEL
#define ROTATE_LABEL

#include <qlabel.h>
#include <qevent.h>
#include <qpainter.h>

namespace Playo3 {
    class RotateLabel : public QLabel {
    public:
        inline RotateLabel(QString text, QWidget * parent) : QLabel(text, parent), isVertical(false) {}
        inline void setVertical(bool vertical) { isVertical = vertical; }
    protected:
        void paintEvent(QPaintEvent *);
        QSize sizeHint() const ;
        QSize minimumSizeHint() const;
    private:
        bool isVertical;
    };
}

#endif // ROTATE_LABEL

