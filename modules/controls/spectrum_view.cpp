#include "spectrum_view.h"
#include "player/player.h"
#include <qelapsedtimer.h>

using namespace Controls;

SpectrumView::SpectrumView(const QString & objName, QWidget * parent) : QToolBar(objName, parent), last_pairs_count(0), type(split_bars) {
    setObjectName(QStringLiteral("tool_Spectrum"));
    setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setAttribute(Qt::WA_OpaquePaintEvent, true);

    connect(Player::instance(), SIGNAL(spectrumChanged(const QList<QVector<int> > &)), this, SLOT(dataUpdated(const QList<QVector<int> > &)));
    connect(Player::instance(), SIGNAL(channelsCountChanged()), this, SLOT(recalcAttrs()));
    connect(this, SIGNAL(movableChanged(bool)), this, SLOT(onMovableChanged(bool)));
    connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onOrientationChanged(Qt::Orientation)));

    updateColors();
    changeBandCount();
    changeHeight(Settings::obj().spectrumHeight());
    changeType(Settings::obj().spectrumType());
    onMovableChanged(isMovable());
}

SpectrumView::~SpectrumView() {}

void SpectrumView::generateContextMenu(QMenu * parent) {
    QAction * act;
    QMenu * spectrMenu = parent -> addMenu(QStringLiteral("Spectrum"));
    spectrMenu -> addAction(toggleViewAction());
    spectrMenu -> addSeparator();

    act = spectrMenu -> addAction(QStringLiteral("Bars view"), this, SLOT(setBarsView()));
    act -> setCheckable(true);
    act -> setChecked(type == bars);

    act = spectrMenu -> addAction(QStringLiteral("Split channel bars view"), this, SLOT(setSplitBarsView()));
    act -> setCheckable(true);
    act -> setChecked(type == split_bars);

    act = spectrMenu -> addAction(QStringLiteral("Waves view"), this, SLOT(setWavesView()));
    act -> setCheckable(true);
    act -> setChecked(type == waves);
}

void SpectrumView::updateColors() {
    QColor c1, c2, c3;
    if (Settings::obj().isCustomColorSpectrum()) {
        c3 = Settings::obj().spectrumColor3();
        c2 = Settings::obj().spectrumColor2();
        c1 = Settings::obj().spectrumColor();
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

void SpectrumView::changeType(SpectrumType newType) {
    Settings::obj().setSpectrumType(newType);
    type = newType;
    Player::instance() -> setSpectrumHeight(peakDimension());
}

void SpectrumView::changeBandCount() {
    Player::instance() -> setSpectrumBandsCount(calcBarCount());
    dataUpdated(Player::instance() -> getDefaultSpectrum());
}

void SpectrumView::changeHeight(int newHeight) {
    setFixedHeight(newHeight);
    setMinimumWidth(100);
    recalcAttrs();
    Player::instance() -> setSpectrumHeight(peakDimension());
}

void SpectrumView::dataUpdated(const QList<QVector<int> > & data) {
    peaks = data;
    pairs = (peaks.length() + 1) / 2;

    if (isVisible()) {
        if (peaks.size() == 1 && type != bars)
            peaks << peaks[0];

        if (pairs != last_pairs_count) {
            recalcAttrs();
            last_pairs_count = pairs;
        }

        update(update_rect);
    }
}

void SpectrumView::onMovableChanged(bool movable) {
     offset = movable ? 10 : 2;
     start_h_offset = paddWidth() + offset;
     recalcAttrs();
}
void SpectrumView::onOrientationChanged(Qt::Orientation /*orientation*/) {
    //TODO: add calculation for vertical orientation
}

void SpectrumView::resizeEvent(QResizeEvent * e) {
    QToolBar::resizeEvent(e);
    changeBandCount();
    recalcAttrs();
}

void SpectrumView::paintEvent(QPaintEvent * event) {
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

void SpectrumView::recalcAttrs() {
    switch(type) {
        case bars:
            bar_width = ((float)width() - start_h_offset - (Player::instance() -> spectrumBandsCount() + 1) * paddWidth()) / Player::instance() -> spectrumBandsCount();
            break;
//case waves:
        default:
            bar_width = ((float)width() - start_h_offset - ((Player::instance() -> getCalcSpectrumBandsCount() * pairs + pairs) * paddWidth()) - ((pairs - 1) * beetweenSpace()))/ pairs / Player::instance() -> getCalcSpectrumBandsCount();             
            break;
    }

    update_rect = QRect(start_h_offset, verticalPadd(), width(), height() - verticalPadd());
}

int SpectrumView::calcBarCount() {
    if (Settings::obj().isAutoBarsAmount())
        return (width() - start_h_offset) / Settings::obj().autoBarWidth();

    return Settings::obj().spectrumBarsCount();
}

int SpectrumView::peakDimension() {
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

void SpectrumView::paintWaves() {
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

void SpectrumView::paintCombo() {
    QPainter painter(this);
    painter.save();

    double accumulate = start_h_offset;
    QRectF rect;

    painter.setBrush(g);
    QVector<QRectF> gRects;

    for(int loop1 = 0; loop1 < peaks[0].length(); loop1++) {
        rect.setCoords(accumulate, start_v1_offset - peaks[0][loop1], (accumulate + bar_width), start_v1_offset);
        gRects.append(rect);
        accumulate += bar_width + paddWidth();
    }

    painter.drawRects(gRects);
    painter.restore();
}

void SpectrumView::paintDuo() {
    QPainter painter(this);
    painter.save();

    double peak, peak2, temp_acc, accumulate = start_h_offset;
    QRectF rect;

    painter.setRenderHint(QPainter::Antialiasing, false);

    QVector<QRectF> gRects;
    QVector<QRectF> ggRects;

    for(int pair = 0; pair < peaks.length(); pair += 2) {
        if (peaks.length() > pair + 1) {
            for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                temp_acc = (accumulate + bar_width);

                peak = peaks[pair][loop1];
                rect.setCoords(accumulate, start_v1_offset - peak, temp_acc, start_v1_offset);
                gRects.append(rect);


                peak2 = peaks[pair + 1][loop1];
                rect.setCoords(accumulate, start_v2_offset, temp_acc, start_v2_offset + peak2);
                ggRects.append(rect);

                accumulate = temp_acc + paddWidth();
            }
        } else {
            for(int loop1 = 0; loop1 < peaks[pair].length(); loop1++) {
                temp_acc = (accumulate + bar_width);

                peak = peaks[pair][loop1];
                rect.setCoords(accumulate, start_v1_offset - peak, temp_acc, start_v1_offset);
                gRects.append(rect);

                accumulate = temp_acc + paddWidth();
            }
        }
        accumulate += beetweenSpace();
    }

    painter.setBrush(g);
    painter.drawRects(gRects);

    painter.setBrush(gg);
    painter.drawRects(ggRects);

    painter.restore();
}
