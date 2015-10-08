#include "item_settings.h"

void ItemSettings::fromJson(const QJsonObject & json) {
    _useGradient = json.value(SETTINGS_USE_GRADIENT_KEY).toBool(true);
    _itemHeight = json.value(SETTINGS_ITEM_HEIGHT_KEY).toInt(18);

    QVariant colorVar = json.value(SETTINGS_DEFAULT_ITEM_COLOR_KEY).toVariant();
    _defaultItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(98, 173, 248);

    colorVar = json.value(SETTINGS_LISTENED_ITEM_COLOR_KEY).toVariant();
    _listenedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(240, 128, 128);

    colorVar = json.value(SETTINGS_LIKED_ITEM_COLOR_KEY).toVariant();
    _likedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(232, 196, 0);

    colorVar = json.value(SETTINGS_PLAYED_ITEM_COLOR_KEY).toVariant();
    _playedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(144, 238, 144);

    colorVar = json.value(SETTINGS_FOLDER_ITEM_COLOR_KEY).toVariant();
    _folderItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(180, 180, 180);

    colorVar = json.value(SETTINGS_ITEM_COLOR_KEY).toVariant();
    _itemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(220, 220, 220);

    __title.fontName = json.value(SETTINGS_ITEM_FONT_NAME_KEY).toString(QStringLiteral("Arial Black"));
    __title.fontSize = json.value(SETTINGS_ITEM_FONT_SIZE_KEY).toInt(8);
    colorVar = json.value(SETTINGS_ITEM_TEXT_COLOR_KEY).toVariant();
    __title.textColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);
    colorVar = json.value(SETTINGS_SELECTED_ITEM_TEXT_COLOR).toVariant();
    __title.selectedTextColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);

    __info.fontName = json.value(SETTINGS_ITEM_INFO_FONT_NAME_KEY).toString(QStringLiteral("Arial Black"));
    __info.fontSize = json.value(SETTINGS_ITEM_INFO_FONT_SIZE_KEY).toInt(7);
    colorVar = json.value(SETTINGS_ITEM_INFO_TEXT_COLOR_KEY).toVariant();
    __info.textColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);
    colorVar = json.value(SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY).toVariant();
    __info.selectedTextColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);
}

void ItemSettings::toJson(QJsonObject & json) {
    json.insert(SETTINGS_USE_GRADIENT_KEY, QJsonValue::fromVariant(_useGradient));
    json.insert(SETTINGS_ITEM_HEIGHT_KEY, QJsonValue::fromVariant(_itemHeight));

    json.insert(SETTINGS_DEFAULT_ITEM_COLOR_KEY, QJsonValue::fromVariant(_defaultItemColor));
    json.insert(SETTINGS_LISTENED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_listenedItemColor));
    json.insert(SETTINGS_LIKED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_likedItemColor));
    json.insert(SETTINGS_PLAYED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_playedItemColor));
    json.insert(SETTINGS_FOLDER_ITEM_COLOR_KEY, QJsonValue::fromVariant(_folderItemColor));
    json.insert(SETTINGS_ITEM_COLOR_KEY, QJsonValue::fromVariant(_itemColor));

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
    else if (_useGradient)
        grad.setColorAt(1, Qt::white);

    return grad;
}

QBrush ItemSettings::unprocessedState(const QRect & rect, bool dark) {
    if (dark) {
        QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());
        grad.setColorAt(0, _folderItemColor);
        grad.setColorAt(0.9, Qt::black);
        return grad;
    }
    else return buildGradient(rect, _folderItemColor, dark);

//    return buildGradient(rect, _folderItemColor, dark);
}
