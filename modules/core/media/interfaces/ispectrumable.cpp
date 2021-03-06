#include "ispectrumable.h"

void ISpectrumable::calcSpectrum() {
    QList<QVector<float> > res;

    if (state() != PlayingState)
        res = sdefault;
    else {
        if (respondToMultichannelSpectrumCalc() && channels_count > 1)
            calcSpectrum(res);
        else {
            QVector<float> layer;
            calcSpectrum(layer);
            res.append(layer);
        }
    }

    emit spectrumChanged(res);
}

void ISpectrumable::channelsCount(int newChannelsCount) {
    if (channels_count == newChannelsCount) return;

    channels_count = newChannelsCount;
    spectrumBandsCount(sbands_count); // recalc predefines
    emit channelsCountChanged();
}

void ISpectrumable::spectrumBandsCount(int bandsCount, bool update) {
    sdefault.clear();
    QVector<float> l;
    l.fill(0, (sbands_count = bandsCount));

    while(sdefault.size() < channels_count)
        sdefault.append(l);

//////////////////  calculate predefined points  ///////////////////////////

    spectrumPoints.clear();
    for (int x = 0, b0 = 0; x < sbands_count; x++) { // 1024 nodes precalc
        int b1 = pow(2, x * 10.0 / (sbands_count - 1));
        if (b1 > 1023) b1 = 1023;
        if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin

        spectrumPoints.append(b1);

        b0 = b1;
    }

    int layerLimit = 1024, gLimit = layerLimit * channels_count;
    int bandGroupsAmount = calcSpectrumBandsGroupCount();

    spectrumComplexPoints.clear();
    for (int x = 0, b0 = 0; x < bandGroupsAmount; x++) {
        int b1 = pow(2, x * 10.0 / (bandGroupsAmount - 1)) * channels_count;
        if (b1 - channels_count <= b0) b1 = b0 + channels_count * 2; // make sure it uses at least 2 FFT bin
        if (b1 > gLimit - 1) b1 = gLimit - 1; // prevent index overflow

        spectrumComplexPoints.append(b1);

        b0 = b1;
    }

    if (update)
        calcSpectrum(); // inform listeners about changes
}
