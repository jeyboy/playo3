#include "spinner.h"

Spinner::Spinner(QString text, int w, int h, QWidget * parent) : QLabel(parent), spineWidth(8), spinePadd(2) {
    img_text = new QStaticText(text);
    QTextOption options(Qt::AlignCenter);
    options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    img_text -> setTextOption(options);
    img_text -> setTextWidth(w);

    outter = QRect(2, 2, w - 2, h - 2);
    spine = QRect(
                outter.left() + spinePadd + spineWidth / 2,
                outter.top() + spinePadd + spineWidth / 2,
                outter.width() - spinePadd - spineWidth / 2,
                outter.height() - spinePadd - spineWidth / 2
                );
    inner = QRect(
                spine.left() + spinePadd + spineWidth / 2,
                spine.top() + spinePadd + spineWidth / 2,
                spine.width() - spinePadd - spineWidth / 2,
                spine.height() - spinePadd - spineWidth / 2
                );

    img = new QPixmap(w, h + 10);
    img_painter = new QPainter(img);
    QPen p(QColor::fromRgb(128, 128, 128, 192));
    p.setWidth(spineWidth);
    p.setCapStyle(Qt::RoundCap);
    img_painter -> setPen(p);

    setPixmap(*img);
}

Spinner::~Spinner() {
    delete img_text;
    delete img_painter;
    delete img;
}

void Spinner::setValue(int percent) {
    img -> fill();
    img_painter -> drawStaticText(0, img -> height() - 10, img_text);

    img_painter -> drawEllipse(outter);
    img_painter -> drawEllipse(inner);
    img_painter -> drawArc(spine, 0, (percent / 100.0) * -5760);
    repaint();
}
