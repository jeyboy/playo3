#ifndef SPECTRUM_SETTINGS
#define SPECTRUM_SETTINGS

#include <qvariant.h>
#include <qcolor.h>
#include <qjsonobject.h>

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

enum SpectrumType {
    bars,
    split_bars
};

class SpectrumSettings {
public:
    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isCustomColorSpectrum() { return _custom_color_spectrum; }
    inline void setCustomColorSpectrum(const bool & use) { _custom_color_spectrum = use; }

    inline bool isAutoBarsAmount() { return _auto_bars_amount; }
    inline void setAutoBarsAmount(const bool & use) { _auto_bars_amount = use; }

    inline int autoBarWidth() { return _auto_bar_width; }
    inline void setAutoBarWidth(const int & new_width) { _auto_bar_width = new_width; }

    inline QColor spectrumColor() { return _spectrum_color; }
    inline void setSpectrumColor(const QColor & new_color) { _spectrum_color = new_color; }

    inline QColor spectrumColor2() { return _spectrum_color2; }
    inline void setSpectrumColor2(const QColor & new_color) { _spectrum_color2 = new_color; }

    inline QColor spectrumColor3() { return _spectrum_color3; }
    inline void setSpectrumColor3(const QColor & new_color) { _spectrum_color3 = new_color; }

    inline int spectrumFreqRate() { return _spectrum_freq_rate; }
    inline void setSpectrumFreqRate(const int & new_rate) { _spectrum_freq_rate = new_rate; }

    inline int spectrumBarsCount() { return _spectrum_bars_count; }
    inline void setSpectrumBarsCount(const int & new_count) { _spectrum_bars_count = new_count; }

    inline int spectrumHeight() { return _spectrum_height; }
    inline void setSpectrumHeight(const int & new_height) { _spectrum_height = new_height; }

    inline int spectrumMultiplier() { return _spectrum_multiplier; }
    inline void setSpectrumMultiplier(const int & new_multiplier) { _spectrum_multiplier = new_multiplier; }

    inline SpectrumType spectrumType() { return _spectrum_type; }
    inline void setSpectrumType(const SpectrumType & new_spectrum_type) { _spectrum_type = new_spectrum_type; }

protected:
    QColor _spectrum_color;
    QColor _spectrum_color2;
    QColor _spectrum_color3;

    int _spectrum_multiplier;
    int _spectrum_freq_rate;
    int _spectrum_bars_count;
    int _spectrum_height;
    int _auto_bar_width;
    SpectrumType _spectrum_type;

    bool _custom_color_spectrum;
    bool _auto_bars_amount;
};

#endif // SPECTRUM_SETTINGS
