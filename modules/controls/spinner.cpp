#include "spinner.h"

#include <qpainter.h>

using namespace Controls;

Spinner::Spinner(const QString & text, int spinner_width, int spinner_height, QWidget * parent, bool fixed_size, const QImage & def_ico)
    : QWidget(parent), img_text(0), spineWidth(spinner_width < 30 ? 4 : 10),
      spinePad(spinner_width < 30 ? 0 : 2), borderWidth(2), continiousLen((15 / 100.0) * -5760),
      w(spinner_width), h(spinner_height), clearPen(0), spinePen(0), show_def_ico(false), def_ico(def_ico)
{

    setAutoFillBackground(false);
    clear();

    if ((show_text = !text.isEmpty())) {
        img_text = new QStaticText(text);
        QTextOption options(Qt::AlignCenter);
        options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        img_text -> setTextOption(options);
        img_text -> setTextWidth(w);
    }

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

    if (fixed_size)
        setFixedSize(w + 2, h + 2);

    setValue(SPINNER_IS_CONTINIOUS); // start from continious state
}

Spinner::~Spinner() {
    delete clearPen;
    delete spinePen;
    delete borderPen;

    delete img_text;
}

void Spinner::setValue(int percent) {
    if (lastVal != percent) {
        continious = (percent == SPINNER_IS_CONTINIOUS);

        lastVal = percent;
        if (continious) {
            continiousPos = 1440; // 12 hours pos
            timer.singleShot(20, this, SLOT(continiousProgression()));
        }
        else update(outter);
    }
}

void Spinner::setValue2(int percent) {
    if (lastVal2 != percent) {
        continious2 = (percent == SPINNER_IS_CONTINIOUS);

        lastVal2 = percent;
        if (continious2) {
            continiousPos2 = 1440; // 12 hours pos
            timer.singleShot(20, this, SLOT(continiousProgression2()));
        }
        else update(inner);
    }
}

void Spinner::clear() {
    continious = continious2 = false;
    lastVal = 0;
    lastVal2 = SPINNER_NOT_SHOW_SECOND;
}

void Spinner::paintEvent(QPaintEvent * e) {
    bool drawSecond = (lastVal2 > SPINNER_NOT_SHOW_SECOND);

    QPainter p(this);
    p.save();
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);

    if (show_def_ico) {
        p.drawImage(outter, def_ico);
    } else {
        if (show_text)
            p.drawStaticText(textPoint, *img_text);

        p.setPen(*borderPen);

        p.drawEllipse(outter);
        p.drawEllipse(inner);

        if (drawSecond)
            p.drawEllipse(ininner);

        p.setPen(*clearPen);
        p.drawEllipse(spine);

        if (drawSecond)
            p.drawEllipse(inner_spine);

        p.setPen(*spinePen);

        if (continious)
            p.drawArc(spine, continiousPos, continiousLen);
        else
            p.drawArc(spine, 1440, (lastVal / 100.0) * -5760);

        if (drawSecond) {
            if (continious2)
                p.drawArc(inner_spine, continiousPos2, continiousLen);
            else
                p.drawArc(inner_spine, 1440, (lastVal2 / 100.0) * -5760);
        }
    }

    p.restore();
    e -> accept();
}

void Spinner::resizeEvent(QResizeEvent * event) {
    QWidget::resizeEvent(event);

    int b = spineWidth / 2 + borderWidth / 2;
    int e = spineWidth + borderWidth;

    outter = QRect(width() / 2 - w / 2, height() / 2 - h / 2, w, h);
    spine = QRect(
                    outter.left() + b, outter.top() + b,
                    outter.width() - e, outter.height() - e
                );

    inner = QRect(
                    spine.left() + b, spine.top() + b,
                    spine.width() - e, spine.height() - e
                );

    inner_spine = QRect(
                    inner.left() + b, inner.top() + b,
                    inner.width() - e, inner.height() - e
                );

    ininner = QRect(
                    inner_spine.left() + b, inner_spine.top() + b,
                    inner_spine.width() - e, inner_spine.height() - e
                );

    textPoint = outter.bottomLeft() + QPoint(0, 2);
}

void Spinner::continiousProgression() {
    update(outter);
    continiousPos -= 80;

    if (continious)
        timer.singleShot(20, this, SLOT(continiousProgression()));
}

void Spinner::continiousProgression2() {
    update(inner);
    continiousPos2 -= 80;

    if (continious2)
        timer2.singleShot(20, this, SLOT(continiousProgression2()));
}
