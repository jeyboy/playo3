#ifndef SPECTRUM_SETTINGS
#define SPECTRUM_SETTINGS

#include <qcolor.h>

#include "misc/spectrum_types.h"
#include "misc/web_utils/json.h"

#define SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY QStringLiteral("customcolor_spectrum")
#define SETTINGS_AUTO_BARS_AMOUNT_KEY QStringLiteral("auto_bars_amount")
#define SETTINGS_AUTO_BARS_WIDTH_KEY QStringLiteral("auto_bar_width")
#define SETTINGS_SPECTRUM_COLOR1_KEY QStringLiteral("spectrum_color")
#define SETTINGS_SPECTRUM_COLOR2_KEY QStringLiteral("spectrum_color2")
#define SETTINGS_SPECTRUM_COLOR3_KEY QStringLiteral("spectrum_color3")
#define SETTINGS_SPECTRUM_RATE_KEY QStringLiteral("spectrum_rate")
#define SETTINGS_SPECTRUM_BARS_COUNT_KEY QStringLiteral("spectrum_bars_count")
#define SETTINGS_SPECTRUM_HEIGHT_KEY QStringLiteral("spectrum_height")
#define SETTINGS_SPECTRUM_TYPE_KEY QStringLiteral("spectrum_type")
#define SETTINGS_SPECTRUM_MULTIPLIER_KEY QStringLiteral("spectrum_multiplier")

class SpectrumSettings {
public:
    void fromJson(const Json::Obj & json);
    void toJson(Json::Obj & json);

    inline bool isCustomColorSpectrum() { return _customColorSpectrum; }
    inline void setCustomColorSpectrum(bool use) { _customColorSpectrum = use; }

    inline bool isAutoBarsAmount() { return _autoBarsAmount; }
    inline void setAutoBarsAmount(bool use) { _autoBarsAmount = use; }

    inline int autoBarWidth() { return _autoBarWidth; }
    inline void setAutoBarWidth(int newWidth) { _autoBarWidth = newWidth; }

    const inline QColor & spectrumColor() const { return _spectrumColor; }
    inline void setSpectrumColor(const QColor & newColor) { _spectrumColor = newColor; }

    const inline QColor & spectrumColor2() const { return _spectrumColor2; }
    inline void setSpectrumColor2(const QColor & newColor) { _spectrumColor2 = newColor; }

    const inline QColor & spectrumColor3() const { return _spectrumColor3; }
    inline void setSpectrumColor3(const QColor & newColor) { _spectrumColor3 = newColor; }

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
    int _autoBarWidth;
    Playo3::SpectrumType _spectrumType;

    bool _customColorSpectrum;
    bool _autoBarsAmount;
};

#endif // SPECTRUM_SETTINGS
