#ifndef SPECTRUM_SETTINGS
#define SPECTRUM_SETTINGS

#include <qcolor.h>
#include "misc/spectrum_types.h"

class SpectrumSettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isCustomColorSpectrum() { return _customColorSpectrum; }
    inline void setCustomColorSpectrum(bool use) { _customColorSpectrum = use; }

    inline QColor spectrumColor() { return _spectrumColor; }
    inline void setSpectrumColor(QColor newColor) { _spectrumColor = newColor; }

    inline QColor spectrumColor2() { return _spectrumColor2; }
    inline void setSpectrumColor2(QColor newColor) { _spectrumColor2 = newColor; }

    inline QColor spectrumColor3() { return _spectrumColor3; }
    inline void setSpectrumColor3(QColor newColor) { _spectrumColor3 = newColor; }

    inline int spectrumFreqRate() { return _spectrumFreqRate; }
    inline void setSpectrumFreqRate(int newRate) { _spectrumFreqRate = newRate; }

    inline int spectrumBarsCount() { return _spectrumBarsCount; }
    inline void setSpectrumBarsCount(int newCount) { _spectrumBarsCount = newCount; }

    inline int spectrumHeight() { return _spectrumHeight; }
    inline void setSpectrumHeight(int newHeight) { _spectrumHeight = newHeight; }

    inline int spectrumMultiplier() { return _spectrumMultiplier; }
    inline void setSpectrumMultiplier(int newMultiplier) { _spectrumMultiplier = newMultiplier; }

    inline Playo3::SpectrumType spectrumType() { return _spectrumType; }
    inline void setSpectrumType(Playo3::SpectrumType newSpectrumType) { _spectrumType = newSpectrumType; }

protected:
    QColor _spectrumColor;
    QColor _spectrumColor2;
    QColor _spectrumColor3;

    int _spectrumMultiplier;
    int _spectrumFreqRate;
    int _spectrumBarsCount;
    int _spectrumHeight;
    Playo3::SpectrumType _spectrumType;
    bool _customColorSpectrum;
};

#endif // SPECTRUM_SETTINGS
