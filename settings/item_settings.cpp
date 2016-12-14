#include "item_settings.h"

void ItemSettings::fromJson(const QJsonObject & json) {
    _open_time_out = json.value(SETTINGS_OPEN_TIMEOUT_KEY).toInt(15);

    _use_gradient = json.value(SETTINGS_USE_GRADIENT_KEY).toBool(true);
    _item_height = json.value(SETTINGS_ITEM_HEIGHT_KEY).toInt(18);

    QVariant color_var = json.value(SETTINGS_DEFAULT_ITEM_COLOR_KEY).toVariant();
    _default_item_color = color_var.isValid() ? color_var.value<QColor>() : QColor(98, 173, 248);

    color_var = json.value(SETTINGS_LISTENED_ITEM_COLOR_KEY).toVariant();
    _listened_item_color = color_var.isValid() ? color_var.value<QColor>() : QColor(240, 128, 128);

    color_var = json.value(SETTINGS_LIKED_ITEM_COLOR_KEY).toVariant();
    _liked_item_color = color_var.isValid() ? color_var.value<QColor>() : QColor(232, 196, 0);

    color_var = json.value(SETTINGS_PLAYED_ITEM_COLOR_KEY).toVariant();
    _played_item_color = color_var.isValid() ? color_var.value<QColor>() : QColor(144, 238, 144);

    color_var = json.value(SETTINGS_FOLDER_ITEM_COLOR_KEY).toVariant();
    _folder_item_color = color_var.isValid() ? color_var.value<QColor>() : QColor(180, 180, 180);

    color_var = json.value(SETTINGS_ITEM_COLOR_KEY).toVariant();
    _item_color = color_var.isValid() ? color_var.value<QColor>() : QColor(220, 220, 220);

    __title.fontName = json.value(SETTINGS_ITEM_FONT_NAME_KEY).toString(QStringLiteral("Arial Black"));
    __title.fontSize = json.value(SETTINGS_ITEM_FONT_SIZE_KEY).toInt(8);
    color_var = json.value(SETTINGS_ITEM_TEXT_COLOR_KEY).toVariant();
    __title.textColor = color_var.isValid() ? color_var.value<QColor>() : QColor(0, 0, 0);
    color_var = json.value(SETTINGS_SELECTED_ITEM_TEXT_COLOR).toVariant();
    __title.selectedTextColor = color_var.isValid() ? color_var.value<QColor>() : QColor(255, 255, 255);

    __info.fontName = json.value(SETTINGS_ITEM_INFO_FONT_NAME_KEY).toString(QStringLiteral("Arial Black"));
    __info.fontSize = json.value(SETTINGS_ITEM_INFO_FONT_SIZE_KEY).toInt(7);
    color_var = json.value(SETTINGS_ITEM_INFO_TEXT_COLOR_KEY).toVariant();
    __info.textColor = color_var.isValid() ? color_var.value<QColor>() : QColor(0, 0, 0);
    color_var = json.value(SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY).toVariant();
    __info.selectedTextColor = color_var.isValid() ? color_var.value<QColor>() : QColor(255, 255, 255);
}

void ItemSettings::toJson(QJsonObject & json) {
    json.insert(SETTINGS_OPEN_TIMEOUT_KEY, QJsonValue::fromVariant(_open_time_out));

    json.insert(SETTINGS_USE_GRADIENT_KEY, QJsonValue::fromVariant(_use_gradient));
    json.insert(SETTINGS_ITEM_HEIGHT_KEY, QJsonValue::fromVariant(_item_height));

    json.insert(SETTINGS_DEFAULT_ITEM_COLOR_KEY, QJsonValue::fromVariant(_default_item_color));
    json.insert(SETTINGS_LISTENED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_listened_item_color));
    json.insert(SETTINGS_LIKED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_liked_item_color));
    json.insert(SETTINGS_PLAYED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_played_item_color));
    json.insert(SETTINGS_FOLDER_ITEM_COLOR_KEY, QJsonValue::fromVariant(_folder_item_color));
    json.insert(SETTINGS_ITEM_COLOR_KEY, QJsonValue::fromVariant(_item_color));

    json.insert(SETTINGS_ITEM_FONT_NAME_KEY, QJsonValue::fromVariant(__title.fontName));
    json.insert(SETTINGS_ITEM_FONT_SIZE_KEY, QJsonValue::fromVariant(__title.fontSize));
    json.insert(SETTINGS_ITEM_TEXT_COLOR_KEY, QJsonValue::fromVariant(__title.textColor));
    json.insert(SETTINGS_SELECTED_ITEM_TEXT_COLOR, QJsonValue::fromVariant(__title.selectedTextColor));

    json.insert(SETTINGS_ITEM_INFO_FONT_NAME_KEY, QJsonValue::fromVariant(__info.fontName));
    json.insert(SETTINGS_ITEM_INFO_FONT_SIZE_KEY, QJsonValue::fromVariant(__info.fontSize));
    json.insert(SETTINGS_ITEM_INFO_TEXT_COLOR_KEY, QJsonValue::fromVariant(__info.textColor));
    json.insert(SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY, QJsonValue::fromVariant(__info.selectedTextColor));
}

QBrush ItemSettings::buildGradient(const QRect & rect, const QColor & color, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    grad.setColorAt(0, color);
    if (dark)
        grad.setColorAt(1, QColor::fromRgba(qRgba(0, 0, 0, 192)));
    else if (_use_gradient)
        grad.setColorAt(1, Qt::white);

    return grad;
}

QBrush ItemSettings::unprocessedState(const QRect & rect, bool dark) {
    if (dark) {
        QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());
        grad.setColorAt(0, _folder_item_color);
        grad.setColorAt(0.9, Qt::black);
        return grad;
    }
    else return buildGradient(rect, _folder_item_color, dark);

//    return buildGradient(rect, _folderItemColor, dark);
}
