#include "spectrum_settings.h"

void SpectrumSettings::fromJson(const QJsonObject & settings) {
    _custom_color_spectrum = settings.value(SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY).toBool(true);

    _auto_bars_amount = settings.value(SETTINGS_AUTO_BARS_AMOUNT_KEY).toBool(true);

    QVariant color_var = settings.value(SETTINGS_SPECTRUM_COLOR1_KEY).toVariant();
    _spectrum_color = color_var.isValid() ? color_var.value<QColor>() : QColor(0, 0, 0);

    color_var = settings.value(SETTINGS_SPECTRUM_COLOR2_KEY).toVariant();
    _spectrum_color2 = color_var.isValid() ? color_var.value<QColor>() : QColor(128, 128, 128);

    color_var = settings.value(SETTINGS_SPECTRUM_COLOR3_KEY).toVariant();
    _spectrum_color3 = color_var.isValid() ? color_var.value<QColor>() : QColor(255, 255, 255);


    _spectrum_freq_rate = settings.value(SETTINGS_SPECTRUM_RATE_KEY).toInt(15);
    _spectrum_bars_count = settings.value(SETTINGS_SPECTRUM_BARS_COUNT_KEY).toInt(30);

    _spectrum_height = settings.value(SETTINGS_SPECTRUM_HEIGHT_KEY).toInt(60);
    _spectrum_type = (SpectrumType)settings.value(SETTINGS_SPECTRUM_TYPE_KEY).toInt(1);

    _spectrum_multiplier = settings.value(SETTINGS_SPECTRUM_MULTIPLIER_KEY).toInt(3);

    _auto_bar_width = settings.value(SETTINGS_AUTO_BARS_WIDTH_KEY).toInt(10);
}

void SpectrumSettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_AUTO_BARS_WIDTH_KEY, QJsonValue::fromVariant(_auto_bar_width));
    settings.insert(SETTINGS_AUTO_BARS_AMOUNT_KEY, QJsonValue::fromVariant(_auto_bars_amount));
    settings.insert(SETTINGS_CUSTOM_COLOR_SPECTRUM_KEY, QJsonValue::fromVariant(_custom_color_spectrum));
    settings.insert(SETTINGS_SPECTRUM_COLOR1_KEY, QJsonValue::fromVariant(_spectrum_color));
    settings.insert(SETTINGS_SPECTRUM_COLOR2_KEY, QJsonValue::fromVariant(_spectrum_color2));
    settings.insert(SETTINGS_SPECTRUM_COLOR3_KEY, QJsonValue::fromVariant(_spectrum_color3));

    settings.insert(SETTINGS_SPECTRUM_RATE_KEY, QJsonValue::fromVariant(_spectrum_freq_rate));
    settings.insert(SETTINGS_SPECTRUM_BARS_COUNT_KEY, QJsonValue::fromVariant(_spectrum_bars_count));
    settings.insert(SETTINGS_SPECTRUM_HEIGHT_KEY, QJsonValue::fromVariant(_spectrum_height));
    settings.insert(SETTINGS_SPECTRUM_TYPE_KEY, QJsonValue::fromVariant(_spectrum_type));
    settings.insert(SETTINGS_SPECTRUM_MULTIPLIER_KEY, QJsonValue::fromVariant(_spectrum_multiplier));
}
