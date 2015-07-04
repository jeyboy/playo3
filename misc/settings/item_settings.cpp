#include "item_settings.h"

void ItemSettings::fromJson(QJsonObject & settings) {
    _useGradient = settings.value("use_gradient").toBool(true);
    _itemHeight = settings.value("item_height").toInt(18);

    QVariant colorVar = settings.value("default_color").toVariant();
    _defaultItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(98, 173, 248);

    colorVar = settings.value("listened_color").toVariant();
    _listenedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(240, 128, 128);

    colorVar = settings.value("liked_color").toVariant();
    _likedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(232, 196, 0);

    colorVar = settings.value("played_color").toVariant();
    _playedItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(144, 238, 144);

    colorVar = settings.value("folder_color").toVariant();
    _folderItemColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(220, 220, 220);

    __title.fontName = settings.value("item_font_name").toString("Arial");
    __title.fontSize = settings.value("item_font_size").toInt(9);
    colorVar = settings.value("item_text_color").toVariant();
    __title.textColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);
    colorVar = settings.value("selected_item_text_color").toVariant();
    __title.selectedTextColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);

    __info.fontName = settings.value("item_info_font_name").toString("Arial");
    __info.fontSize = settings.value("item_info_font_size").toInt(8);
    colorVar = settings.value("item_info_text_color").toVariant();
    __info.textColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(0, 0, 0);
    colorVar = settings.value("selected_item_info_text_color").toVariant();
    __info.selectedTextColor = colorVar.isValid() ? colorVar.value<QColor>() : QColor(255, 255, 255);
}

void ItemSettings::toJson(QJsonObject & settings) {
    settings.insert("use_gradient", QJsonValue::fromVariant(_useGradient));
    settings.insert("item_height", QJsonValue::fromVariant(_itemHeight));

    settings.insert("default_color", QJsonValue::fromVariant(_defaultItemColor));
    settings.insert("listened_color", QJsonValue::fromVariant(_listenedItemColor));
    settings.insert("liked_color", QJsonValue::fromVariant(_likedItemColor));
    settings.insert("played_color", QJsonValue::fromVariant(_playedItemColor));
    settings.insert("folder_color", QJsonValue::fromVariant(_folderItemColor));

    settings.insert("item_font_name", QJsonValue::fromVariant(__title.fontName));
    settings.insert("item_font_size", QJsonValue::fromVariant(__title.fontSize));
    settings.insert("item_text_color", QJsonValue::fromVariant(__title.textColor));
    settings.insert("selected_item_text_color", QJsonValue::fromVariant(__title.selectedTextColor));

    settings.insert("item_info_font_name", QJsonValue::fromVariant(__info.fontName));
    settings.insert("item_info_font_size", QJsonValue::fromVariant(__info.fontSize));
    settings.insert("item_info_text_color", QJsonValue::fromVariant(__info.textColor));
    settings.insert("selected_item_info_text_color", QJsonValue::fromVariant(__info.selectedTextColor));
}

QBrush ItemSettings::buildGradient(QRect rect, QColor color, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    grad.setColorAt(0, color);
    if (dark)
        grad.setColorAt(1, QColor::fromRgba(qRgba(0, 0, 0, 192)));
    else if (_useGradient)
        grad.setColorAt(1, Qt::white);

    return grad;
}

QBrush ItemSettings::unprocessedState(QRect rect, bool dark) {
    if (dark) {
        QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());
        grad.setColorAt(0, _folderItemColor);
        grad.setColorAt(0.9, Qt::black);
        return grad;
    }
    else return buildGradient(rect, _folderItemColor, dark);

//    return buildGradient(rect, _folderItemColor, dark);
}
