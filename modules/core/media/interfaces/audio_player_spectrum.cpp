#include "audio_player_spectrum.h"

using namespace AudioPlayer;

float Spectrum::fastSqrt(float x) {
  unsigned int i = *(unsigned int*) &x;

  // adjust bias
  i  += 127 << 23;
  // approximation of square root
  i >>= 1;

  return *(float*) &i;
}

void Spectrum::calcSpectrum() {
    if (spectrumHeight > 0) {
        if (state() == StoppedState) {
            channelsCount = DEFAULT_CHANNELS_COUNT;
            emit spectrumChanged(defaultSpectrum);
        } else {
            if (Settings::instance() -> spectrumType() == bars) {
                QList<QVector<int> > res;
                res.append(getSpectrum());
                emit spectrumChanged(res);
            }
            else emit spectrumChanged(getComplexSpectrum());
        }
    }
}

void Spectrum::setSpectrumBandsCount(int bandsCount) {
    _spectrumBandsCount = bandsCount;
    defaultSpectrum.clear();
    QVector<int> l;
    l.fill(defaultSpectrumLevel, _spectrumBandsCount);

    while(defaultSpectrum.size() < DEFAULT_CHANNELS_COUNT)
        defaultSpectrum.append(l);

//////////////////  calculate predefined points  ///////////////////////////

    spectrumPoints.clear();
    for (int x = 0, b0 = 0; x < _spectrumBandsCount; x++) { // 1024 nodes precalc
        int b1 = pow(2, x * 10.0 / (_spectrumBandsCount - 1));
        if (b1 > 1023) b1 = 1023;
        if (b1 <= b0) b1 = b0 + 1; // make sure it uses at least 1 FFT bin

        spectrumPoints.append(b1);

        b0 = b1;
    }

    int layerLimit = 1024, gLimit = layerLimit * channelsCount;
    int workSpectrumBandsCount = getCalcSpectrumBandsCount();

    spectrumComplexPoints.clear();
    for (int x = 0, b0 = 0; x < workSpectrumBandsCount; x++) {
        int b1 = pow(2, x * 10.0 / (workSpectrumBandsCount - 1)) * channelsCount;
        if (b1 - channelsCount <= b0) b1 = b0 + channelsCount * 2; // make sure it uses at least 2 FFT bin
        if (b1 > gLimit - 1) b1 = gLimit - 1; // prevent index overflow

        spectrumComplexPoints.append(b1);

        b0 = b1;
    }
}

QVector<int> Spectrum::getSpectrum() {
    float fft[1024];
    BASS_ChannelGetData(chId(), fft, BASS_DATA_FFT2048);
    QVector<int> res;
    int spectrumMultiplicity = Settings::instance() -> spectrumMultiplier() * spectrumHeight;

    int b0 = 0, x, y;

    for (x = 0; x < _spectrumBandsCount; x++) {
        float peak = 0;
        int b1 = spectrumPoints[x];
        for (; b0 < b1; b0++)
            if (peak < fft[1 + b0])
                peak = fft[1 + b0];

        y = fastSqrt(peak) * spectrumMultiplicity + defaultSpectrumLevel; // 4 // scale it (sqrt to make low values more visible)
        if (y > spectrumHeight) y = spectrumHeight; // cap it

        res.append(y);
    }

    return res;
}

QList<QVector<int> > Spectrum::getComplexSpectrum() {
    QList<QVector<int> > res;

    if (channelsCount == 1) {
        res.append(getSpectrum());
        return res;
    }

    int layerLimit = 1024, gLimit = layerLimit * channelsCount;
    int spectrumMultiplicity = Settings::instance() -> spectrumMultiplier() * spectrumHeight;
    int workSpectrumBandsCount = getCalcSpectrumBandsCount();
    float fft[gLimit];
    BASS_ChannelGetData(chId(), fft, BASS_DATA_FFT2048 | BASS_DATA_FFT_INDIVIDUAL | BASS_DATA_FFT_REMOVEDC);

    QVector<float> peaks;
    int b0 = 0, x, y, z, peakNum;

    for (x = 0; x < channelsCount; x++)
        res.append(QVector<int>());

    for (x = 0; x < workSpectrumBandsCount; x++) {
        peaks.fill(0, channelsCount);

        int b1 = spectrumComplexPoints[x];

        for (; b0 < b1; b0++) {
            peakNum = b0 % channelsCount;
            if (peaks[peakNum] < fft[b0])
                peaks[peakNum] = fft[b0];
        }

        for (z = 0; z < channelsCount; z++) {
            y = fastSqrt(peaks[z]) * spectrumMultiplicity + defaultSpectrumLevel; // 4 // scale it (sqrt to make low values more visible)
            if (y > spectrumHeight) y = spectrumHeight; // cap it

            res[z].append(y);
        }
    }

    return res;
}
