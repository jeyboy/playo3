#include "spectrum.h"
#include "media/player.h"
#include <QLinearGradient>
#include "misc/settings.h"

using namespace Playo3;

Spectrum::Spectrum(QWidget * parent) : QToolBar("Spectrum", parent), isWave(false) {
    setObjectName("tool_Spectrum");
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    connect(Player::instance(), SIGNAL(spectrumChanged(QList<QVector<int> >)), this, SLOT(dataUpdated(QList<QVector<int> >)));
    connect(this, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));
    connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onOrientationChanged(Qt::Orientation)));
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(onVisibilityChanged(bool)));
    //void topLevelChanged(bool topLevel);

    bandCountChanged(Settings::instance() -> getSpectrumBarsCount());
    heightChanged(Settings::instance() -> getSpectrumHeight());
}

Spectrum::~Spectrum() {

}

void Spectrum::resizeEvent(QResizeEvent * e) {
    QToolBar::resizeEvent(e);
}

int Spectrum::workHeight() {
    if (Settings::instance() -> getSpectrumCombo())
        return height() - verticalPadd() * 2;
    else
        return (height() - (verticalPadd() * 2 + 8)) / 2;
}

void Spectrum::bandCountChanged(int newCount) {
    Player::instance() -> setSpectrumBandsCount(newCount);
    peaks = Player::instance() -> getDefaultSpectrum();
}

void Spectrum::heightChanged(int newHeight) {
    setFixedHeight(newHeight);
    setMinimumWidth(200);
    Player::instance() -> setSpectrumHeight(workHeight());
}

void Spectrum::dataUpdated(QList<QVector<int> > bars) {
    peaks = bars;
    if (isVisible())
        repaint();
}

void Spectrum::onMovableChanged(bool movable) {

}
void Spectrum::onOrientationChanged(Qt::Orientation orientation) {

}
//void Spectrum::onTopLevelChanged(bool topLevel);
void Spectrum::onVisibilityChanged(bool visible) {

}

void Spectrum::paintEvent(QPaintEvent * event) {
    QToolBar::paintEvent(event);

    if (Settings::instance() -> getSpectrumCombo())
        paintCombo();
    else
        paintDuo();
}

void Spectrum::paintCombo() {
    QPainter painter(this);
    painter.save();

    int offset = isMovable() ? 10 : 0, padd = paddWidth();
    double peak, accumulate = padd + offset;
    float bar_width = ((float)width() - offset - (Player::instance() -> spectrumBandsCount() + 1) * padd) / Player::instance() -> spectrumBandsCount();
    QRectF rect;

    QLinearGradient g(bar_width / 2, verticalPadd(), bar_width / 2, workHeight());

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

    g.setColorAt(0.1, c3);
    g.setColorAt(0.5, c2);
    g.setColorAt(0.6, c2);
    g.setColorAt(1, c1);

    painter.setBrush(g);

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        peak = peaks[0][loop1];
        rect.setCoords(accumulate, height() - 6 - peak, (accumulate + bar_width), height() - 6);
        painter.drawRect(rect);
        accumulate += bar_width + padd;
    }
    painter.restore();
}

void Spectrum::paintDuo() {
    QPainter painter(this);
    painter.save();

    int pairs = (peaks.length() + 1) / 2, padd = paddWidth(); // / pairs;
    int offset = isMovable() ? 10 : 0;
    int work_bars_count = Player::instance() -> getCalcSpectrumBandsCount();
    double peak, peak2, temp_acc, accumulate = padd + offset + 2, beetween_space = 6;
    double bar_width = ((float)width() - 2 - accumulate - ((work_bars_count * pairs + pairs) * padd) - ((pairs - 1) * beetween_space))/ pairs / work_bars_count;
    float bar_height = workHeight() + 3, first_bar_place =  bar_height + verticalPadd() + 1, sec_bar_place = bar_height + verticalPadd() + 3; // 2px gap between vertical bars
    QRectF rect;

    if (isWave) { // waves
        painter.setRenderHint(QPainter::Antialiasing, true);

        for(int pair = 0; pair < peaks.length(); pair += 2) {
            peaks[pair].prepend(0);
            peaks[pair + 1].prepend(0);

            if (peaks.length() > pair + 1) {
                {
                    QPainterPath lpath;
                    QPainterPath rpath;

                    lpath.moveTo(accumulate, first_bar_place - peaks[pair][0]);
                    rpath.moveTo(accumulate, sec_bar_place + peaks[pair + 1][0]);

                    for(int loop1 = 0; loop1 < peaks[pair].length() - 2; loop1++) {
                        temp_acc = (accumulate + bar_width);

                        lpath.quadTo(
                                    accumulate, first_bar_place - peaks[pair][loop1],
                                    accumulate + padd + bar_width, first_bar_place - peaks[pair][loop1 + 1]
//                                    accumulate + (padd + bar_width) * 2, first_bar_place - peaks[pair][loop1 + 2]
                                    );
                        rpath.quadTo(
                                    accumulate, sec_bar_place + peaks[pair + 1][loop1],
                                    accumulate + padd + bar_width, sec_bar_place + peaks[pair + 1][loop1 + 1]
//                                    accumulate + (padd + bar_width) * 2, sec_bar_place + peaks[pair + 1][loop1 + 2]
                                    );

                        accumulate = temp_acc + padd;
                    }

                    painter.drawPath(lpath);
                    painter.drawPath(rpath);
                }
            } else {
                {
                    QPainterPath lpath;
                    lpath.moveTo(accumulate, first_bar_place - peaks[pair][0]);

                    for(int loop1 = 1; loop1 < peaks[pair].length() - 2; loop1++) {
                        temp_acc = (accumulate + bar_width);
                        lpath.quadTo(
                                    accumulate, first_bar_place - peaks[pair][loop1],
                                    accumulate + padd + bar_width, first_bar_place - peaks[pair][loop1 + 1]
//                                    accumulate + (padd + bar_width) * 2, first_bar_place - peaks[pair][loop1 + 2]
                                    );
                        accumulate = temp_acc + padd;
                    }

                    painter.drawPath(lpath);
                }
            }
            accumulate += beetween_space;
        }
    } else { // bars
        QLinearGradient g(bar_width / 2, first_bar_place - bar_height, bar_width / 2, first_bar_place);
        QLinearGradient gg(bar_width / 2, sec_bar_place + bar_height, bar_width / 2, sec_bar_place);

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

        painter.setRenderHint(QPainter::Antialiasing, false);

        for(int pair = 0; pair < peaks.length(); pair += 2) {
            if (peaks.length() > pair + 1) {
                for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                    temp_acc = (accumulate + bar_width);

                    peak = peaks[pair][loop1];
                    rect.setCoords(accumulate, first_bar_place - peak, temp_acc, first_bar_place);
                    painter.setBrush(g);
                    painter.drawRect(rect);


                    peak2 = peaks[pair + 1][loop1];
                    rect.setCoords(accumulate, sec_bar_place, temp_acc, sec_bar_place + peak2);
                    painter.setBrush(gg);
                    painter.drawRect(rect);

                    accumulate = temp_acc + padd;
                }
            } else {
                for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                    temp_acc = (accumulate + bar_width);

                    peak = peaks[pair][loop1];
                    rect.setCoords(accumulate, first_bar_place - peak, temp_acc, first_bar_place);
                    painter.setBrush(g);
                    painter.drawRect(rect);

                    accumulate = temp_acc + padd;
                }
            }
            accumulate += beetween_space;
        }
    }

    painter.restore();
}



//void Spectrum::paintDuo() {
//    QPainter painter(this);
//    painter.save();
//    int offset = isMovable() ? 10 : 0, padd = paddWidth();
//    double peak, peak2, temp_acc, accumulate = padd + offset;
//    float bar_width = ((float)width() - offset - (bars_count + 1) * padd) / bars_count;
//    float bar_height = workHeight() + 3, first_bar_place =  bar_height + 3, sec_bar_place = bar_height + 9;
//    QRectF rect;

//    QLinearGradient g(bar_width / 2, 0, bar_width / 2, bar_height);
//    QLinearGradient gg(bar_width / 2, sec_bar_place + bar_height, bar_width / 2, sec_bar_place);

//    if (Settings::instance() -> getMonocolorSpectrum()) {
//        g.setColorAt(0, Settings::instance() -> getSpectrumColor2());
//        g.setColorAt(1, Settings::instance() -> getSpectrumColor());

//        gg.setColorAt(0, Settings::instance() -> getSpectrumColor2());
//        gg.setColorAt(1, Settings::instance() -> getSpectrumColor());
//    } else {
//        g.setColorAt(0.1, Qt::red);
//        g.setColorAt(0.5, Qt::yellow);
//        g.setColorAt(0.6, Qt::yellow);
//        g.setColorAt(1, Qt::darkGreen);

//        gg.setColorAt(0.1, Qt::red);
//        gg.setColorAt(0.5, Qt::yellow);
//        gg.setColorAt(0.6, Qt::yellow);
//        gg.setColorAt(1, Qt::darkGreen);
//    }

//    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
//        temp_acc = (accumulate + bar_width);

//        peak = peaks[0][loop1];
//        rect.setCoords(accumulate, first_bar_place - peak, temp_acc, first_bar_place);
//        painter.fillRect(rect, g);
//        painter.drawRect(rect);

//        peak2 = peaks[1][loop1];
//        rect.setCoords(accumulate, sec_bar_place, temp_acc, sec_bar_place + peak2);
//        painter.fillRect(rect, gg);
//        painter.drawRect(rect);

//        accumulate = temp_acc + padd;
//    }
//    painter.restore();
//}
