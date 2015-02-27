#include "settings.h"

Settings *Settings::self = 0;

Settings *Settings::instance() {
    if(!self)
        self = new Settings();
    return self;
}

void Settings::fromJson(QJsonObject settingsObj) {
    setHotKeys(settingsObj.value("hotkeys").toObject());
    showCheckbox = settingsObj.value("show_checkboxes").toBool(true);

    spoilOnActivation = settingsObj.value("spoil_on_activation").toBool(true);
    alertOnFolderDeletion = settingsObj.value("alert_on_folder_deletion").toBool(true);

    showInfo = settingsObj.value("show_info").toBool(true);
    showSystemIcons = settingsObj.value("show_system_icons").toBool(false);
    useGradient = settingsObj.value("use_gradient").toBool(true);

    QVariant defaultColor1Var = settingsObj.value("default_color1").toVariant();
    defaultColor1 = defaultColor1Var.isValid() ? defaultColor1Var.value<QColor>() : QColor(98, 173, 248);

    QVariant listenedColor1Var = settingsObj.value("listened_color1").toVariant();
    listenedColor1 = listenedColor1Var.isValid() ? listenedColor1Var.value<QColor>() : QColor(240, 128, 128);

    QVariant likedColor1Var = settingsObj.value("liked_color1").toVariant();
    likedColor1 = likedColor1Var.isValid() ? likedColor1Var.value<QColor>() : QColor(232, 196, 0);

    QVariant playedColor1Var = settingsObj.value("played_color1").toVariant();
    playedColor1 = playedColor1Var.isValid() ? playedColor1Var.value<QColor>() : QColor(144, 238, 144);

    QVariant folderColor1Var = settingsObj.value("folder_color1").toVariant();
    folderColor1 = folderColor1Var.isValid() ? folderColor1Var.value<QColor>() : QColor(220, 220, 220);

    itemFontName = settingsObj.value("item_font_name").toString("Arial");
    itemFontSize = settingsObj.value("item_font_size").toInt(9);

    QVariant itemTextColorVar = settingsObj.value("item_text_color").toVariant();
    itemTextColor = itemTextColorVar.isValid() ? itemTextColorVar.value<QColor>() : QColor(0, 0, 0);

    QVariant selectedItemTextColorVar = settingsObj.value("selected_item_text_color").toVariant();
    selectedItemTextColor = selectedItemTextColorVar.isValid() ? selectedItemTextColorVar.value<QColor>() : QColor(255, 255, 255);

    itemInfoFontName = settingsObj.value("item_info_font_name").toString("Arial");
    itemInfoFontSize = settingsObj.value("item_info_font_size").toInt(7);

    itemTextColorVar = settingsObj.value("item_info_text_color").toVariant();
    itemInfoColor = itemTextColorVar.isValid() ? itemTextColorVar.value<QColor>() : QColor(0, 0, 0);

    selectedItemTextColorVar = settingsObj.value("selected_item_info_text_color").toVariant();
    selectedItemInfoColor = selectedItemTextColorVar.isValid() ? selectedItemTextColorVar.value<QColor>() : QColor(255, 255, 255);

    tabPosition = settingsObj.value("tab_position").toInt(0);

    itemHeight = settingsObj.value("item_height").toInt(18);
    treeIndentation = settingsObj.value("tree_indentation").toInt(12);

    customcolorSpectrum = settingsObj.value("customcolor_spectrum").toBool(false);

    QVariant specColorVar = settingsObj.value("spectrum_color").toVariant();
    spectrumColor = specColorVar.isValid() ? specColorVar.value<QColor>() : QColor(0, 0, 0);

    QVariant specColorVar2 = settingsObj.value("spectrum_color2").toVariant();
    spectrumColor2 = specColorVar2.isValid() ? specColorVar2.value<QColor>() : QColor(128, 128, 128);

    QVariant specColorVar3 = settingsObj.value("spectrum_color3").toVariant();
    spectrumColor3 = specColorVar3.isValid() ? specColorVar3.value<QColor>() : QColor(255, 255, 255);


    spectrumFreqRate = settingsObj.value("spectrum_rate").toInt(20);
    spectrumBarsCount = settingsObj.value("spectrum_bars_count").toInt(30);

    spectrumHeight = settingsObj.value("spectrum_height").toInt(60);
    spectrumType = (Playo3::SpectrumType)settingsObj.value("spectrum_type").toInt(1);

    spectrumMultiplier = settingsObj.value("spectrum_multiplier").toInt(3);
}

QJsonObject Settings::toJson() {
    QJsonObject ret = QJsonObject();

    ret.insert("hotkeys", hotkeysJson());

    ret.insert("show_checkboxes", QJsonValue::fromVariant(showCheckbox));

    ret.insert("spoil_on_activation", QJsonValue::fromVariant(spoilOnActivation));
    ret.insert("alert_on_folder_deletion", QJsonValue::fromVariant(alertOnFolderDeletion));

    ret.insert("show_info", QJsonValue::fromVariant(showInfo));
    ret.insert("show_system_icons", QJsonValue::fromVariant(showSystemIcons));

    ret.insert("default_color1", QJsonValue::fromVariant(defaultColor1));
    ret.insert("listened_color1", QJsonValue::fromVariant(listenedColor1));
    ret.insert("liked_color1", QJsonValue::fromVariant(likedColor1));
    ret.insert("played_color1", QJsonValue::fromVariant(playedColor1));
    ret.insert("folder_color1", QJsonValue::fromVariant(folderColor1));


    ret.insert("item_font_name", QJsonValue::fromVariant(itemFontName));
    ret.insert("item_font_size", QJsonValue::fromVariant(itemFontSize));
    ret.insert("item_text_color", QJsonValue::fromVariant(itemTextColor));
    ret.insert("selected_item_text_color", QJsonValue::fromVariant(selectedItemTextColor));

    ret.insert("item_info_font_name", QJsonValue::fromVariant(itemInfoFontName));
    ret.insert("item_info_font_size", QJsonValue::fromVariant(itemInfoFontSize));
    ret.insert("item_info_text_color", QJsonValue::fromVariant(itemInfoColor));
    ret.insert("selected_item_info_text_color", QJsonValue::fromVariant(selectedItemInfoColor));

    ret.insert("tab_position", QJsonValue::fromVariant(tabPosition));
    ret.insert("item_height", QJsonValue::fromVariant(itemHeight));
    ret.insert("tree_indentation", QJsonValue::fromVariant(treeIndentation));

    ret.insert("customcolor_spectrum", QJsonValue::fromVariant(customcolorSpectrum));
    ret.insert("spectrum_color", QJsonValue::fromVariant(spectrumColor));
    ret.insert("spectrum_color2", QJsonValue::fromVariant(spectrumColor2));
    ret.insert("spectrum_color3", QJsonValue::fromVariant(spectrumColor3));

    ret.insert("spectrum_rate", QJsonValue::fromVariant(spectrumFreqRate));
    ret.insert("spectrum_bars_count", QJsonValue::fromVariant(spectrumBarsCount));
    ret.insert("spectrum_height", QJsonValue::fromVariant(spectrumHeight));
    ret.insert("spectrum_type", QJsonValue::fromVariant(spectrumType));
    ret.insert("spectrum_multiplier", QJsonValue::fromVariant(spectrumMultiplier));

    return ret;
}
