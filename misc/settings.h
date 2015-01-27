#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QFont>
#include <QLinearGradient>

#include "modules/data_struct/hotkeys/hotkey_model_item.h"
#include "misc/hotkey_types.h"

class Settings {
public:
    ~Settings() {

    }

    static Settings * instance();
    static void close() {
        delete self;
    }

    QBrush buildGradient(QRect rect, QColor color, bool dark);
    QBrush defaultState(QRect rect, bool dark);
    QBrush listenedState(QRect rect, bool dark);
    QBrush likedState(QRect rect, bool dark);
    QBrush playedState(QRect rect, bool dark);
    QBrush unprocessedState(QRect rect, bool dark);

    bool isUseGradient() const;
    void setUseGradient(bool use);

    QColor getDefaultColor() const;
    void setDefaultColor(QColor newColor);

    QColor getListenedColor() const;
    void setListenedColor(QColor newColor);

    QColor getLikedColor() const;
    void setLikedColor(QColor newColor);

    QColor getPlayedColor() const;
    void setPlayedColor(QColor newColor);

    QColor getFolderColor() const;
    void setFolderColor(QColor newColor);

    bool isShowSystemIcons() const;
    void setShowSystemIcons(bool show);

    bool isShowInfo() const;
    void setShowInfo(bool show);

    bool isSpoilOnActivation() const;
    void setSpoilOnActivation(bool show);

    bool isMetricShow() const;
    void setMetricShow(bool show);

    bool isCheckboxShow() const;
    void setCheckboxShow(bool show);

    QString getDownloadPath() const;
    void setDownloadPath(QString newPath);


    QString getItemFontName();
    void setItemFontName(QString newFontName);

    int getItemFontSize();
    void setItemFontSize(int newFontSize);

    QColor getItemTextColor();
    void setItemTextColor(QColor newColor);

    QColor getSelectedItemTextColor();
    void setSelectedItemTextColor(QColor newColor);

    QString getItemInfoFontName();
    void setItemInfoFontName(QString newFontName);

    int getItemInfoFontSize();
    void setItemInfoFontSize(int newFontSize);

    QColor getItemInfoTextColor();
    void setItemInfoTextColor(QColor newColor);

    QColor getSelectedItemInfoTextColor();
    void setSelectedItemInfoTextColor(QColor newColor);

    int getTabPosition();
    void setTabPosition(int newPositionsType);

    int getItemHeight();
    void setItemHeight(int newHeight);

    bool getScrollButtonUsage();
    void setScrollButtonUsage(bool use);

    bool isCustomColorSpectrum();
    void setCustomColorSpectrum(bool use);

    QColor getSpectrumColor();
    void setSpectrumColor(QColor newColor);

    QColor getSpectrumColor2();
    void setSpectrumColor2(QColor newColor);

    QColor getSpectrumColor3();
    void setSpectrumColor3(QColor newColor);

    int getSpectrumFreqRate();
    void setSpectrumFreqRate(int newRate);

    int getSpectrumBarsCount();
    void setSpectrumBarsCount(int newCount);

    int getSpectrumHeight();
    void setSpectrumHeight(int newHeight);

    bool getSpectrumCombo();
    void setSpectrumCombo(bool newState);

    int getSpectrumMultiplier();
    void setSpectrumMultiplier(int newMultiplier);

    int getTotalItemHeight();

    int getIconHeight();

    QList<HotkeyModelItem *> * getHotKeys() const;
    void setHotKeys(QList<HotkeyModelItem *>);

    void fromJson(QJsonObject settingsObj = QJsonObject());
    QJsonObject toJson();

    QFont getItemFont();
    QFont getItemInfoFont();

private:
    Settings() {
        humanizeHotkeyText = QHash<int, QString>();
        humanizeHotkeyText.insert(HOTKEY_NEXT, "Activate next item");
        humanizeHotkeyText.insert(HOTKEY_NEXT_AND_DELETE, "Remove current and activate next");
        humanizeHotkeyText.insert(HOTKEY_PREV, "Activate prev item");
        humanizeHotkeyText.insert(HOTKEY_PLAY, "Play/pause");
        humanizeHotkeyText.insert(HOTKEY_STOP, "Stop");
        humanizeHotkeyText.insert(HOTKEY_SETTINGS, "Active tab settings");
        humanizeHotkeyText.insert(HOTKEY_POS_SLIDE_FORWARD, "Move position slider at 10 % forward");
        humanizeHotkeyText.insert(HOTKEY_POS_SLIDE_BACKWARD, "Move position slider at 10 % backward");
        humanizeHotkeyText.insert(HOTKEY_VOL_SLIDE_FORWARD, "Move volume slider at 10 % forward");
        humanizeHotkeyText.insert(HOTKEY_VOL_SLIDE_BACKWARD, "Move volume slider at 10 % backward");
    }

    void setHotKeys(QJsonObject hotkeysHash = QJsonObject());
    QJsonObject hotkeysJson() const;

    static Settings *self;

    QString downloadPath;
    bool showSystemIcons;
    bool showInfo;
    bool showCheckbox;
    bool showMetric;
    bool spoilOnActivation;
    bool useGradient;
    bool useScrollButtons;
    QJsonObject hotkeys;
    QHash<int, QString> humanizeHotkeyText;

    QColor defaultColor1, listenedColor1, likedColor1, playedColor1, folderColor1;

    int tabPosition;

    int itemHeight;

    QString itemFontName;
    int itemFontSize;
    QColor itemTextColor;
    QColor selectedItemTextColor;

    QString itemInfoFontName;
    int itemInfoFontSize;
    QColor itemInfoColor;
    QColor selectedItemInfoColor;

    QColor spectrumColor;
    QColor spectrumColor2;
    QColor spectrumColor3;
    int spectrumMultiplier;
    int spectrumFreqRate;
    int spectrumBarsCount;
    int spectrumHeight;
    bool customcolorSpectrum;
    bool comboSpectrum;
};

#endif // SETTINGS_H
