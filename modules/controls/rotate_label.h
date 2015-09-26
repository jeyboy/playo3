#ifndef ROTATE_LABEL
#define ROTATE_LABEL

#include <qlabel.h>
#include <qevent.h>
#include <qpainter.h>

namespace Controls {
    class RotateLabel : public QLabel {
    public:
        inline RotateLabel(QString text, QWidget * parent) : QLabel(text, parent), is_vertical(false) {
            setContentsMargins(0,0,0,0);
        }
        inline void setVertical(bool vertical) { is_vertical = vertical; }
        inline bool isVertical() { return is_vertical; }
    protected:
        void paintEvent(QPaintEvent *);
        QSize sizeHint() const ;
        QSize minimumSizeHint() const;
    private:
        bool is_vertical;
    };
}

#endif // ROTATE_LABEL

