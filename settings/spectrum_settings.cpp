#include "spectrum_settings.h"

void SpectrumSettings::fromJson(const QJsonObject & settings) {
    _customColorSpectrum = settings.value(SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY).toBool(true);

    _autoBarsAmount = settings.value(SETTINGS_AUTO_BARS_AMOUNT_KEY).toBool(true);

    QVariant colorVar = settings.value(SETTINGS_SPECTRUM_COLOR1_KEY).toVariant();
    _spectrumColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);

    colorVar = settings.value(SETTINGS_SPECTRUM_COLOR2_KEY).toVariant();
    _spectrumColor2 = colorVar.isValid() ? colorVar.value<QColor>() : QColor(128, 128, 128);

    colorVar = settings.value(SETTINGS_SPECTRUM_COLOR3_KEY).toVariant();
    _spectrumColor3 = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);


    _spectrumFreqRate = settings.value(SETTINGS_SPECTRUM_RATE_KEY).toInt(15);
    _spectrumBarsCount = settings.value(SETTINGS_SPECTRUM_BARS_COUNT_KEY).toInt(30);

    _spectrumHeight = settings.value(SETTINGS_SPECTRUM_HEIGHT_KEY).toInt(60);
    _spectrumType = (SpectrumType)settings.value(SETTINGS_SPECTRUM_TYPE_KEY).toInt(1);

    _spectrumMultiplier = settings.value(SETTINGS_SPECTRUM_MULTIPLIER_KEY).toInt(3);

    _autoBarWidth = settings.value(SETTINGS_AUTO_BARS_WIDTH_KEY).toInt(10);
}

void SpectrumSettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_AUTO_BARS_WIDTH_KEY, QJsonValue::fromVariant(_autoBarWidth));
    settings.insert(SETTINGS_AUTO_BARS_AMOUNT_KEY, QJsonValue::fromVariant(_autoBarsAmount));
    settings.insert(SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY, QJsonValue::fromVariant(_customColorSpectrum));
    settings.insert(SETTINGS_SPECTRUM_COLOR1_KEY, QJsonValue::fromVariant(_spectrumColor));
    settings.insert(SETTINGS_SPECTRUM_COLOR2_KEY, QJsonValue::fromVariant(_spectrumColor2));
    settings.insert(SETTINGS_SPECTRUM_COLOR3_KEY, QJsonValue::fromVariant(_spectrumColor3));

    settings.insert(SETTINGS_SPECTRUM_RATE_KEY, QJsonValue::fromVariant(_spectrumFreqRate));
    settings.insert(SETTINGS_SPECTRUM_BARS_COUNT_KEY, QJsonValue::fromVariant(_spectrumBarsCount));
    settings.insert(SETTINGS_SPECTRUM_HEIGHT_KEY, QJsonValue::fromVariant(_spectrumHeight));
    settings.insert(SETTINGS_SPECTRUM_TYPE_KEY, QJsonValue::fromVariant(_spectrumType));
    settings.insert(SETTINGS_SPECTRUM_MULTIPLIER_KEY, QJsonValue::fromVariant(_spectrumMultiplier));
}
