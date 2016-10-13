#ifndef ITEM_SETTINGS
#define ITEM_SETTINGS

#include <qvariant.h>
#include <qfont.h>
#include <qbrush.h>
#include <qjsonobject.h>

#define SETTINGS_OPEN_TIMEOUT_KEY QStringLiteral("open_time_out_key")
#define SETTINGS_USE_GRADIENT_KEY QStringLiteral("use_gradient")
#define SETTINGS_ITEM_HEIGHT_KEY QStringLiteral("item_height")
#define SETTINGS_DEFAULT_ITEM_COLOR_KEY QStringLiteral("default_color")
#define SETTINGS_LISTENED_ITEM_COLOR_KEY QStringLiteral("listened_color")
#define SETTINGS_LIKED_ITEM_COLOR_KEY QStringLiteral("liked_color")
#define SETTINGS_PLAYED_ITEM_COLOR_KEY QStringLiteral("played_color")
#define SETTINGS_FOLDER_ITEM_COLOR_KEY QStringLiteral("folder_color")
#define SETTINGS_ITEM_COLOR_KEY QStringLiteral("item_color")
#define SETTINGS_ITEM_FONT_NAME_KEY QStringLiteral("item_font_name")
#define SETTINGS_ITEM_FONT_SIZE_KEY QStringLiteral("item_font_size")
#define SETTINGS_ITEM_TEXT_COLOR_KEY QStringLiteral("item_text_color")
#define SETTINGS_SELECTED_ITEM_TEXT_COLOR QStringLiteral("selected_item_text_color")
#define SETTINGS_ITEM_INFO_FONT_NAME_KEY QStringLiteral("item_info_font_name")
#define SETTINGS_ITEM_INFO_FONT_SIZE_KEY QStringLiteral("item_info_font_size")
#define SETTINGS_ITEM_INFO_TEXT_COLOR_KEY QStringLiteral("item_info_text_color")
#define SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY QStringLiteral("selected_item_info_text_color")

struct ItemTextAttrs {
    QString fontName;
    int fontSize;
    QColor textColor;
    QColor selectedTextColor;
};

class ItemSettings {
public:
    void fromJson(const QJsonObject & json);
    void toJson(QJsonObject & json);

    QBrush buildGradient(const QRect & rect, const QColor & color, bool dark);

    inline QBrush defaultState(const QRect & rect, bool dark) { return buildGradient(rect, _defaultItemColor, dark); }
    inline QBrush listenedState(const QRect & rect, bool dark) { return buildGradient(rect, _listenedItemColor, dark); }
    inline QBrush likedState(const QRect & rect, bool dark) { return buildGradient(rect, _likedItemColor, dark); }
    inline QBrush playedState(const QRect & rect, bool dark) { return buildGradient(rect, _playedItemColor, dark); }
    inline QBrush itemState(const QRect & rect, bool dark) { return buildGradient(rect, _itemColor, dark); }

    QBrush unprocessedState(const QRect & rect, bool dark);

    inline bool isUseGradient() const { return _useGradient; }
    inline void setUseGradient(bool use) { _useGradient = use; }

    inline QColor defaultItemColor() const { return _defaultItemColor; }
    inline void setDefaultItemColor(QColor newColor) { _defaultItemColor = newColor; }

    inline QColor listenedItemColor() const { return _listenedItemColor; }
    inline void setListenedItemColor(QColor newColor) { _listenedItemColor = newColor; }

    inline QColor likedItemColor() const { return _likedItemColor; }
    inline void setLikedItemColor(QColor newColor) { _likedItemColor = newColor; }

    inline QColor playedItemColor() const { return _playedItemColor; }
    inline void setPlayedItemColor(QColor newColor) { _playedItemColor = newColor; }

    inline QColor folderItemColor() const { return _folderItemColor; }
    inline void setFolderItemColor(QColor newColor) { _folderItemColor = newColor; }

    inline QColor itemColor() const { return _itemColor; }
    inline void setItemColor(QColor newColor) { _itemColor = newColor; }

    inline int itemHeight() { return _itemHeight; }
    inline void setItemHeight(int newHeight) { _itemHeight = newHeight; }


    inline QString itemFontName() { return __title.fontName; }
    inline void setItemFontName(QString newFontName) { __title.fontName = newFontName; }

    inline int itemFontSize() { return __title.fontSize; }
    inline void setItemFontSize(int newFontSize) { __title.fontSize = newFontSize; }

    inline QColor itemTextColor() { return __title.textColor; }
    inline void setItemTextColor(QColor newColor) { __title.textColor = newColor; }

    inline QColor selectedItemTextColor() { return __title.selectedTextColor; }
    inline void setSelectedItemTextColor(QColor newColor) { __title.selectedTextColor = newColor; }


    inline QString itemInfoFontName() { return __info.fontName; }
    inline void setItemInfoFontName(QString newFontName) { __info.fontName = newFontName; }

    inline int itemInfoFontSize() { return __info.fontSize; }
    inline void setItemInfoFontSize(int newFontSize) { __info.fontSize = newFontSize; }

    inline QColor itemInfoTextColor() { return __info.textColor; }
    inline void setItemInfoTextColor(QColor newColor) { __info.textColor = newColor; }

    inline QColor selectedItemInfoTextColor() { return __info.selectedTextColor; }
    inline void setSelectedItemInfoTextColor(QColor newColor) { __info.selectedTextColor = newColor; }

    inline QFont itemFont() { return QFont(itemFontName(), itemFontSize(), QFont::Normal); }
    inline QFont itemInfoFont() { return QFont(itemInfoFontName(), itemInfoFontSize(), QFont::Bold); }

    inline float openTimeOut() { return _openTimeOut; }
    inline void setOpenTimeOut(int newTimeOut) { _openTimeOut = newTimeOut; }
protected:
    bool _useGradient;
    int _itemHeight;
    QColor _defaultItemColor, _listenedItemColor, _likedItemColor, _playedItemColor, _folderItemColor, _itemColor;
    ItemTextAttrs __title, __info;
    float _openTimeOut;
};

#endif // ITEM_SETTINGS
