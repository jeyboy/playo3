#include "spectrum_settings.h"

void SpectrumSettings::fromJson(const Json::Obj & json) {
    _customColorSpectrum = json.value(SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY).toBool(true);

    _autoBarsAmount = json.value(SETTINGS_AUTO_BARS_AMOUNT_KEY).toBool(true);

    QVariant colorVar = json.value(SETTINGS_SPECTRUM_COLOR1_KEY).toVariant();
    _spectrumColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);

    colorVar = json.value(SETTINGS_SPECTRUM_COLOR2_KEY).toVariant();
    _spectrumColor2 = colorVar.isValid() ? colorVar.value<QColor>() : QColor(128, 128, 128);

    colorVar = json.value(SETTINGS_SPECTRUM_COLOR3_KEY).toVariant();
    _spectrumColor3 = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);


    _spectrumFreqRate = json.value(SETTINGS_SPECTRUM_RATE_KEY).toInt(15);
    _spectrumBarsCount = json.value(SETTINGS_SPECTRUM_BARS_COUNT_KEY).toInt(30);

    _spectrumHeight = json.value(SETTINGS_SPECTRUM_HEIGHT_KEY).toInt(60);
    _spectrumType = (Playo3::SpectrumType)json.value(SETTINGS_SPECTRUM_TYPE_KEY).toInt(1);

    _spectrumMultiplier = json.value(SETTINGS_SPECTRUM_MULTIPLIER_KEY).toInt(3);

    _autoBarWidth = json.value(SETTINGS_AUTO_BARS_WIDTH_KEY).toInt(10);
}

void SpectrumSettings::toJson(const Json::Obj & json) {
    json.insert(SETTINGS_AUTO_BARS_WIDTH_KEY, QJsonValue::fromVariant(_autoBarWidth));
    json.insert(SETTINGS_AUTO_BARS_AMOUNT_KEY, QJsonValue::fromVariant(_autoBarsAmount));
    json.insert(SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY, QJsonValue::fromVariant(_customColorSpectrum));
    json.insert(SETTINGS_SPECTRUM_COLOR1_KEY, QJsonValue::fromVariant(_spectrumColor));
    json.insert(SETTINGS_SPECTRUM_COLOR2_KEY, QJsonValue::fromVariant(_spectrumColor2));
    json.insert(SETTINGS_SPECTRUM_COLOR3_KEY, QJsonValue::fromVariant(_spectrumColor3));

    json.insert(SETTINGS_SPECTRUM_RATE_KEY, QJsonValue::fromVariant(_spectrumFreqRate));
    json.insert(SETTINGS_SPECTRUM_BARS_COUNT_KEY, QJsonValue::fromVariant(_spectrumBarsCount));
    json.insert(SETTINGS_SPECTRUM_HEIGHT_KEY, QJsonValue::fromVariant(_spectrumHeight));
    json.insert(SETTINGS_SPECTRUM_TYPE_KEY, QJsonValue::fromVariant(_spectrumType));
    json.insert(SETTINGS_SPECTRUM_MULTIPLIER_KEY, QJsonValue::fromVariant(_spectrumMultiplier));
}
