#include "spinner.h"
#include <qdebug.h>

Spinner::Spinner(QString text, int w, int h, QWidget * parent) : QWidget(parent),
        spineWidth(10), spinePad(2), borderWidth(2), lastVal(0), continiousLen((15 / 100.0) * -5760), clearPen(0), spinePen(0), continious(false) {

    setAutoFillBackground(false);

    img_text = new QStaticText(text);
    QTextOption options(Qt::AlignCenter);
    options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    img_text -> setTextOption(options);
    img_text -> setTextWidth(w);

    outter = QRect(2, 2, w - 4, h - 4);
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
        continious = !(lastVal < 0);

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
    int xoffset = (e -> size().width() - e -> oldSize().width()) / 2;
    int yoffset = (e -> size().height() - e -> oldSize().height()) / 2;

    outter.moveTopLeft(QPoint(outter.left() + xoffset, outter.top() + yoffset));
    spine.moveTopLeft(QPoint(spine.left() + xoffset, spine.top() + yoffset));
    inner.moveTopLeft(QPoint(inner.left() + xoffset, inner.top() + yoffset));

    textPoint = outter.bottomLeft() + QPoint(0, 2);

    QWidget::resizeEvent(e)
;}

void Spinner::continiousProgression() {
    continiousPos -= 64;
    update();

    if (continious)
        timer.singleShot(20, this, SLOT(continiousProgression()));
}
