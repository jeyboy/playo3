#include "item_settings.h"

void ItemSettings::fromJson(const Json::Obj & settings) {
    _useGradient = settings.value(SETTINGS_USE_GRADIENT_KEY).toBool(true);
    _itemHeight = settings.value(SETTINGS_ITEM_HEIGHT_KEY).toInt(18);

    QVariant colorVar = settings.value(SETTINGS_DEFAULT_ITEM_COLOR_KEY).toVariant();
    _defaultItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(98, 173, 248);

    colorVar = settings.value(SETTINGS_LISTENED_ITEM_COLOR_KEY).toVariant();
    _listenedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(240, 128, 128);

    colorVar = settings.value(SETTINGS_LIKED_ITEM_COLOR_KEY).toVariant();
    _likedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(232, 196, 0);

    colorVar = settings.value(SETTINGS_PLAYED_ITEM_COLOR_KEY).toVariant();
    _playedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(144, 238, 144);

    colorVar = settings.value(SETTINGS_FOLDER_ITEM_COLOR_KEY).toVariant();
    _folderItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(180, 180, 180);

    colorVar = settings.value(SETTINGS_ITEM_COLOR_KEY).toVariant();
    _itemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(220, 220, 220);

    __title.fontName = settings.value(SETTINGS_ITEM_FONT_NAME_KEY).toString(QStringLiteral("Arial Black"));
    __title.fontSize = settings.value(SETTINGS_ITEM_FONT_SIZE_KEY).toInt(8);
    colorVar = settings.value(SETTINGS_ITEM_TEXT_COLOR_KEY).toVariant();
    __title.textColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);
    colorVar = settings.value(SETTINGS_SELECTED_ITEM_TEXT_COLOR).toVariant();
    __title.selectedTextColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);

    __info.fontName = settings.value(SETTINGS_ITEM_INFO_FONT_NAME_KEY).toString(QStringLiteral("Arial Black"));
    __info.fontSize = settings.value(SETTINGS_ITEM_INFO_FONT_SIZE_KEY).toInt(7);
    colorVar = settings.value(SETTINGS_ITEM_INFO_TEXT_COLOR_KEY).toVariant();
    __info.textColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);
    colorVar = settings.value(SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY).toVariant();
    __info.selectedTextColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);
}

void ItemSettings::toJson(Json::Obj & settings) {
    settings.insert(SETTINGS_USE_GRADIENT_KEY, QJsonValue::fromVariant(_useGradient));
    settings.insert(SETTINGS_ITEM_HEIGHT_KEY, QJsonValue::fromVariant(_itemHeight));

    settings.insert(SETTINGS_DEFAULT_ITEM_COLOR_KEY, QJsonValue::fromVariant(_defaultItemColor));
    settings.insert(SETTINGS_LISTENED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_listenedItemColor));
    settings.insert(SETTINGS_LIKED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_likedItemColor));
    settings.insert(SETTINGS_PLAYED_ITEM_COLOR_KEY, QJsonValue::fromVariant(_playedItemColor));
    settings.insert(SETTINGS_FOLDER_ITEM_COLOR_KEY, QJsonValue::fromVariant(_folderItemColor));
    settings.insert(SETTINGS_ITEM_COLOR_KEY, QJsonValue::fromVariant(_itemColor));

    settings.insert(SETTINGS_ITEM_FONT_NAME_KEY, QJsonValue::fromVariant(__title.fontName));
    settings.insert(SETTINGS_ITEM_FONT_SIZE_KEY, QJsonValue::fromVariant(__title.fontSize));
    settings.insert(SETTINGS_ITEM_TEXT_COLOR_KEY, QJsonValue::fromVariant(__title.textColor));
    settings.insert(SETTINGS_SELECTED_ITEM_TEXT_COLOR, QJsonValue::fromVariant(__title.selectedTextColor));

    settings.insert(SETTINGS_ITEM_INFO_FONT_NAME_KEY, QJsonValue::fromVariant(__info.fontName));
    settings.insert(SETTINGS_ITEM_INFO_FONT_SIZE_KEY, QJsonValue::fromVariant(__info.fontSize));
    settings.insert(SETTINGS_ITEM_INFO_TEXT_COLOR_KEY, QJsonValue::fromVariant(__info.textColor));
    settings.insert(SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY, QJsonValue::fromVariant(__info.selectedTextColor));
}

QBrush ItemSettings::buildGradient(const QRect & rect, const QColor & color, bool dark) const {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    grad.setColorAt(0, color);
    if (dark)
        grad.setColorAt(1, QColor::fromRgba(qRgba(0, 0, 0, 192)));
    else if (_useGradient)
        grad.setColorAt(1, Qt::white);

    return grad;
}

QBrush ItemSettings::unprocessedState(const QRect & rect, bool dark) const {
    if (dark) {
        QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());
        grad.setColorAt(0, _folderItemColor);
        grad.setColorAt(0.9, Qt::black);
        return grad;
    }
    else return buildGradient(rect, _folderItemColor, dark);

//    return buildGradient(rect, _folderItemColor, dark);
}
