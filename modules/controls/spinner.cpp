#include "spinner.h"
#include <qdebug.h>

Spinner::Spinner(QString text, int spinner_width, int spinner_height, QWidget * parent) : QWidget(parent),
        spineWidth(10), spinePad(2), borderWidth(2), lastVal(0), continiousLen((15 / 100.0) * -5760),
        w(spinner_width), h(spinner_height), clearPen(0), spinePen(0), continious(false) {

    setAutoFillBackground(false);

    img_text = new QStaticText(text);
    QTextOption options(Qt::AlignCenter);
    options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    img_text -> setTextOption(options);
    img_text -> setTextWidth(w);

    borderPen = new QPen(QColor::fromRgb(32, 32, 32, 224));
    borderPen -> setWidth(borderWidth);
    borderPen -> setCosmetic(true);

    clearPen = new QPen(QColor::fromRgb(255, 255, 255, 128));
    clearPen -> setCosmetic(true);
    clearPen -> setWidth(spineWidth - 1);

    spinePen = new QPen(QColor::fromRgb(0, 0, 255, 224));
    spinePen -> setWidth(spineWidth - spinePad * 2);
    spinePen -> setCosmetic(true);
    spinePen -> setCapStyle(Qt::RoundCap);
}

Spinner::~Spinner() {
    delete clearPen;
    delete spinePen;
    delete borderPen;

    delete img_text;
}

void Spinner::setValue(int percent) {
    if (lastVal != percent) {
        continious = (percent < 0 && lastVal >= 0);

        if (continious) {
            continiousPos = 1440;
            timer.singleShot(20, this, SLOT(continiousProgression()));
        }

        lastVal = percent;
        update();
    }
}

void Spinner::paintEvent(QPaintEvent * e) {
    QPainter p(this);
    p.save();
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);

    p.drawStaticText(textPoint, *img_text);

    p.setPen(*borderPen);

    p.drawEllipse(outter);
    p.drawEllipse(inner);

    p.setPen(*clearPen);
    p.drawEllipse(spine);

    p.setPen(*spinePen);

    if (continious)
        p.drawArc(spine, continiousPos, continiousLen);
    else
        p.drawArc(spine, 1440, (lastVal / 100.0) * -5760);

    p.restore();
    e -> accept();
}

void Spinner::resizeEvent(QResizeEvent * e) {
    outter = QRect(width() / 2 - w / 2, height() / 2 - h / 2, w, h);
    spine = QRect(
                outter.left() + spineWidth / 2 + borderWidth / 2,
                outter.top() + spineWidth / 2 + borderWidth / 2,
                outter.width() - spineWidth - borderWidth,
                outter.height() - spineWidth - borderWidth
                );

    inner = QRect(
                spine.left() + spineWidth / 2 + borderWidth / 2,
                spine.top() + spineWidth / 2 + borderWidth / 2,
                spine.width() - spineWidth - borderWidth,
                spine.height() - spineWidth - borderWidth
                );

    textPoint = outter.bottomLeft() + QPoint(0, 2);

    QWidget::resizeEvent(e)
;}

void Spinner::continiousProgression() {
    continiousPos -= 80;
    update();

    if (continious)
        timer.singleShot(20, this, SLOT(continiousProgression()));
}
