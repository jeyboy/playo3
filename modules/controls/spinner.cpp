#include "spinner.h"

Spinner::Spinner(QString text, int w, int h, QWidget * parent) : QLabel(parent), spineWidth(8), spinePadd(2), borderWidth(2) {
    img_text = new QStaticText(text);
    QTextOption options(Qt::AlignCenter);
    options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    img_text -> setTextOption(options);
    img_text -> setTextWidth(w);

    QRect outter = QRect(2, 2, w - 4, h - 4);
    spine = QRect(
                outter.left() + spinePadd + spineWidth / 4 + borderWidth / 2,
                outter.top() + spinePadd + spineWidth / 4 + borderWidth / 2,
                outter.width() - spinePadd - spineWidth / 2 - borderWidth * 2,
                outter.height() - spinePadd - spineWidth / 2 - borderWidth * 2
                );

    QRect inner = QRect(
                spine.left() + spinePadd + borderWidth * 1.5,
                spine.top() + spinePadd + borderWidth * 1.5,
                spine.width() - spinePadd - borderWidth * 4,
                spine.height() - spinePadd - borderWidth * 4
                );

    img = new QPixmap(w, h + 20);
    img -> fill(QColor::fromRgb(255, 255, 255, 0));
    img_painter = new QPainter(img);

    img_painter -> drawStaticText(0, img -> height() - 18, *img_text);

    QPen pp(QColor::fromRgb(32, 32, 32, 224));
    pp.setWidth(borderWidth);
    img_painter -> setPen(pp);
    img_painter -> setRenderHint(QPainter::HighQualityAntialiasing, true);

    img_painter -> drawEllipse(outter);
    img_painter -> drawEllipse(inner);

    QPen ppp(QColor::fromRgb(255, 255, 255, 128));
    ppp.setWidth(spineWidth);
    ppp.setCapStyle(Qt::RoundCap);
    img_painter -> setPen(ppp);
    img_painter -> drawArc(spine, 0, 5760);

    QPen p(QColor::fromRgb(0, 0, 255, 224));
    p.setWidth(spineWidth - 2);
    p.setCapStyle(Qt::RoundCap);
    img_painter -> setPen(p);

//    img_painter -> drawArc(spine, 0, 5760);

    setAlignment(Qt::AlignCenter);
    setPixmap(*img);
}

Spinner::~Spinner() {
    delete img_text;
    delete img_painter;
    delete img;
}

void Spinner::setValue(int percent) {
//    img_painter -> drawArc(spine, 0, 5760);
    img_painter -> drawArc(spine, 0, (percent / 100.0) * -5760);
    repaint();
//    setPixmap(*img);
}
