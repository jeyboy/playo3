#include "spinner.h"

Spinner::Spinner(QString text, int w, int h, QWidget * parent) : QLabel(parent),
        spineWidth(8), spinePadd(2), borderWidth(2), lastVal(0), clearPen(0), spinePen(0), continious(false) {
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
    pp.setCosmetic(true);
    img_painter -> setPen(pp);
    img_painter -> setRenderHint(QPainter::HighQualityAntialiasing, true);

    img_painter -> drawEllipse(outter);
    img_painter -> drawEllipse(inner);

    clearPen = new QPen(QColor::fromRgb(255, 255, 255, 128));
    clearPen -> setCosmetic(true);
    clearPen -> setWidth(spineWidth - 2);
    clearSpine();

    spinePen = new QPen(QColor::fromRgb(0, 0, 255, 224));
    spinePen -> setWidth(spineWidth - 5);
    spinePen -> setCosmetic(true);
    spinePen -> setCapStyle(Qt::RoundCap);

    setAlignment(Qt::AlignCenter);
    setPixmap(*img);
}

Spinner::~Spinner() {
    delete clearPen;
    delete spinePen;
    delete img_text;
    delete img_painter;
    delete img;
}

void Spinner::setValue(int percent) {
    if (percent < 0) {
        continious = !(lastVal < 0);

        if (continious) {
            continiousPos = 1440;
            timer.singleShot(28, this, SLOT(continiousProgression()));
        }
    }
    else if (lastVal != percent)
        drawSpine(1440, percent);

    lastVal = percent;
}

void Spinner::continiousProgression() {
    drawSpine(continiousPos--, 15);

    if (continious)
        timer.singleShot(28, this, SLOT(continiousProgression()));
}

void Spinner::drawSpine(int start, int percent) {
    clearSpine();
    img_painter -> setPen(*spinePen);

    img_painter -> drawArc(spine, start, (percent / 100.0) * -5760);
    setPixmap(*img);
}

void Spinner::clearSpine() {
    img_painter -> setPen(*clearPen);
    img_painter -> drawArc(spine, 0, 5760);
}
