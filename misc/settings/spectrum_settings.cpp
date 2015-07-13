#include "spectrum_settings.h"

void SpectrumSettings::fromJson(QJsonObject & settings) {
    _customColorSpectrum = settings.value("customcolor_spectrum").toBool(false);

    _autoBarsAmount = settings.value("auto_bars_amount").toBool(true);

    QVariant colorVar = settings.value("spectrum_color").toVariant();
    _spectrumColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);

    colorVar = settings.value("spectrum_color2").toVariant();
    _spectrumColor2 = colorVar.isValid() ? colorVar.value<QColor>() : QColor(128, 128, 128);

    colorVar = settings.value("spectrum_color3").toVariant();
    _spectrumColor3 = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);


    _spectrumFreqRate = settings.value("spectrum_rate").toInt(15);
    _spectrumBarsCount = settings.value("spectrum_bars_count").toInt(30);

    _spectrumHeight = settings.value("spectrum_height").toInt(60);
    _spectrumType = (Playo3::SpectrumType)settings.value("spectrum_type").toInt(1);

    _spectrumMultiplier = settings.value("spectrum_multiplier").toInt(3);

    _autoBarWidth = settings.value("auto_bar_width").toInt(10);
}

void SpectrumSettings::toJson(QJsonObject & settings) {
    settings.insert("auto_bar_width", QJsonValue::fromVariant(_autoBarWidth));
    settings.insert("auto_bars_amount", QJsonValue::fromVariant(_autoBarsAmount));
    settings.insert("customcolor_spectrum", QJsonValue::fromVariant(_customColorSpectrum));
    settings.insert("spectrum_color", QJsonValue::fromVariant(_spectrumColor));
    settings.insert("spectrum_color2", QJsonValue::fromVariant(_spectrumColor2));
    settings.insert("spectrum_color3", QJsonValue::fromVariant(_spectrumColor3));

    settings.insert("spectrum_rate", QJsonValue::fromVariant(_spectrumFreqRate));
    settings.insert("spectrum_bars_count", QJsonValue::fromVariant(_spectrumBarsCount));
    settings.insert("spectrum_height", QJsonValue::fromVariant(_spectrumHeight));
    settings.insert("spectrum_type", QJsonValue::fromVariant(_spectrumType));
    settings.insert("spectrum_multiplier", QJsonValue::fromVariant(_spectrumMultiplier));
}
