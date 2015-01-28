#include "spectrum.h"
#include "media/player.h"
#include "misc/settings.h"

using namespace Playo3;

Spectrum::Spectrum(QWidget * parent) : QToolBar("Spectrum", parent), last_pairs_count(0), type(split_bars) {
    setObjectName("tool_Spectrum");
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    connect(Player::instance(), SIGNAL(spectrumChanged(QList<QVector<int> >)), this, SLOT(dataUpdated(QList<QVector<int> >)));
    connect(this, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));
    connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onOrientationChanged(Qt::Orientation)));

    updateColors();
    changeBandCount(Settings::instance() -> getSpectrumBarsCount());
    changeHeight(Settings::instance() -> getSpectrumHeight());
}

Spectrum::~Spectrum() {

}

void Spectrum::updateColors() {
    QColor c1, c2, c3;
    if (Settings::instance() -> isCustomColorSpectrum()) {
        c3 = Settings::instance() -> getSpectrumColor3();
        c2 = Settings::instance() -> getSpectrumColor2();
        c1 = Settings::instance() -> getSpectrumColor();
    } else {
        c3 = QColor::fromRgb(0, 170, 255);
        c2 = QColor::fromRgb(0, 136, 199);
        c1 = QColor::fromRgb(0, 115, 165);
    }

    gg.setColorAt(0.1, c3);
    gg.setColorAt(0.5, c2);
    gg.setColorAt(0.6, c2);
    gg.setColorAt(1, c1);

    g.setColorAt(0.1, c3);
    g.setColorAt(0.5, c2);
    g.setColorAt(0.6, c2);
    g.setColorAt(1, c1);
}

void Spectrum::changeType(SpectrumType newType) {
    type = newType;
    Player::instance() -> setSpectrumHeight(peakDimension());
}

void Spectrum::changeBandCount(int newCount) {
    Player::instance() -> setSpectrumBandsCount(newCount);
    dataUpdated(Player::instance() -> getDefaultSpectrum());
}

void Spectrum::changeHeight(int newHeight) {
    setFixedHeight(newHeight);
    setMinimumWidth(200);
    recalcAttrs();
    Player::instance() -> setSpectrumHeight(peakDimension());
}

void Spectrum::dataUpdated(QList<QVector<int> > bars) {
    peaks = bars;
    pairs = (peaks.length() + 1) / 2;

    if (pairs != last_pairs_count) {
        recalcAttrs();
        last_pairs_count = pairs;
    }

    if (isVisible())
        repaint();
}

void Spectrum::onMovableChanged(bool movable) {
     offset = movable ? 10 : 0;
     start_h_offset = paddWidth() + offset;
     recalcAttrs();
}
void Spectrum::onOrientationChanged(Qt::Orientation /*orientation*/) {
    //TODO: add calculation for vertical orientation
}

void Spectrum::resizeEvent(QResizeEvent * e) {
    QToolBar::resizeEvent(e);
    recalcAttrs();
}

void Spectrum::paintEvent(QPaintEvent * event) {
    QToolBar::paintEvent(event);

    switch(type) {
        case bars: {
            paintCombo();
        break;}
        case waves: {
            paintWaves();
        break;}
        default: paintDuo();
    }
}

void Spectrum::recalcAttrs() {
    switch(type) {
        case bars:
            bar_width = ((float)width() - start_h_offset - (Player::instance() -> spectrumBandsCount() + 1) * paddWidth()) / Player::instance() -> spectrumBandsCount();
            break;
//case waves:
        default:
            bar_width = ((float)width() - start_h_offset - ((Player::instance() -> getCalcSpectrumBandsCount() * pairs + pairs) * paddWidth()) - ((pairs - 1) * beetweenSpace()))/ pairs / Player::instance() -> getCalcSpectrumBandsCount();
            break;
    }
}

int Spectrum::peakDimension() {
    int h;
    int halfBarWidth = bar_width / 2;

    switch(type) {
        case bars:
            start_v1_offset = height() - verticalPadd();
            g.setStart(halfBarWidth, verticalPadd());
            g.setFinalStop(halfBarWidth, start_v1_offset);
            return start_v1_offset - verticalPadd();
//        case waves:

        default:
            h = (height() - (verticalPadd() * 2 + 8)) / 2;
            start_v1_offset = h + verticalPadd() + 4; // +3
            start_v2_offset = start_v1_offset + 2; // 2px gap between bars

            g.setStart(halfBarWidth, start_v1_offset - h);
            g.setFinalStop(halfBarWidth, start_v1_offset);

            gg.setStart(halfBarWidth, start_v2_offset + h);
            gg.setFinalStop(halfBarWidth, start_v2_offset);
            return h;
    }
}

void Spectrum::paintWaves() {
    QPainter painter(this);
    painter.save();

    double temp_acc, accumulate = start_h_offset;
    painter.setRenderHint(QPainter::Antialiasing, true);

    for(int pair = 0; pair < peaks.length(); pair += 2) {
        peaks[pair].prepend(0);
        peaks[pair + 1].prepend(0);

        if (peaks.length() > pair + 1) {
            {
                QPainterPath lpath;
                QPainterPath rpath;

                lpath.moveTo(accumulate, start_v1_offset - peaks[pair][0]);
                rpath.moveTo(accumulate, start_v2_offset + peaks[pair + 1][0]);

                for(int loop1 = 0; loop1 < peaks[pair].length() - 2; loop1++) {
                    temp_acc = accumulate + bar_width + paddWidth();

                    lpath.quadTo(
                                accumulate, start_v1_offset - peaks[pair][loop1],
                                temp_acc, start_v1_offset - peaks[pair][loop1 + 1]
//                                    accumulate + (paddWidth() + bar_width) * 2, first_bar_place - peaks[pair][loop1 + 2]
                                );
                    rpath.quadTo(
                                accumulate, start_v2_offset + peaks[pair + 1][loop1],
                                temp_acc, start_v2_offset + peaks[pair + 1][loop1 + 1]
//                                    accumulate + (paddWidth() + bar_width) * 2, sec_bar_place + peaks[pair + 1][loop1 + 2]
                                );

                    accumulate = temp_acc;
                }

                painter.setBrush(g);
                painter.drawPath(lpath);
                painter.setBrush(gg);
                painter.drawPath(rpath);
            }
        } else {
            {
                QPainterPath lpath;
                lpath.moveTo(accumulate, start_v1_offset - peaks[pair][0]);

                for(int loop1 = 1; loop1 < peaks[pair].length() - 2; loop1++) {
                    temp_acc = accumulate + bar_width + paddWidth();
                    lpath.quadTo(
                                accumulate, start_v1_offset - peaks[pair][loop1],
                                temp_acc, start_v1_offset - peaks[pair][loop1 + 1]
//                                    accumulate + (paddWidth() + bar_width) * 2, start_v1_offset - peaks[pair][loop1 + 2]
                                );
                    accumulate = temp_acc;
                }

                painter.setBrush(g);
                painter.drawPath(lpath);
            }
        }
        accumulate += beetweenSpace();
    }

    painter.restore();
}

void Spectrum::paintCombo() {
    QPainter painter(this);
    painter.save();

    double peak, accumulate = start_h_offset;
    QRectF rect;

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        rect.setCoords(accumulate, start_v1_offset - peak, (accumulate + bar_width), start_v1_offset);
        painter.drawRect(rect);
        accumulate += bar_width + paddWidth();
    }
    painter.restore();
}

void Spectrum::paintDuo() {
    QPainter painter(this);
    painter.save();

    double peak, peak2, temp_acc, accumulate = start_h_offset;
    QRectF rect;

    painter.setRenderHint(QPainter::Antialiasing, false);

    for(int pair = 0; pair < peaks.length(); pair += 2) {
        if (peaks.length() > pair + 1) {
            for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                temp_acc = (accumulate + bar_width);

                peak = peaks[pair][loop1];
                rect.setCoords(accumulate, start_v1_offset - peak, temp_acc, start_v1_offset);
                painter.setBrush(g);
                painter.drawRect(rect);


                peak2 = peaks[pair + 1][loop1];
                rect.setCoords(accumulate, start_v2_offset, temp_acc, start_v2_offset + peak2);
                painter.setBrush(gg);
                painter.drawRect(rect);

                accumulate = temp_acc + paddWidth();
            }
        } else {
            for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                temp_acc = (accumulate + bar_width);

                peak = peaks[pair][loop1];
                rect.setCoords(accumulate, start_v1_offset - peak, temp_acc, start_v1_offset);
                painter.setBrush(g);
                painter.drawRect(rect);

                accumulate = temp_acc + paddWidth();
            }
        }
        accumulate += beetweenSpace();
    }

    painter.restore();
}
