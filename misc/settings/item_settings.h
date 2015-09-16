#ifndef ITEM_SETTINGS
#define ITEM_SETTINGS

#include "misc/web_utils/json.h"

#include <qfont.h>
#include <qbrush.h>

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
    void fromJson(const Json::Obj & settings = Json::Obj());
    void toJson(Json::Obj & settings);

    QBrush & buildGradient(const QRect & rect, const QColor & color, bool dark) const;

    //TODO: need to caching brush for rect for some speed up
    inline QBrush & defaultState(const QRect & rect, bool dark) const { return buildGradient(rect, _defaultItemColor, dark); }
    inline QBrush & listenedState(const QRect &  rect, bool dark) const { return buildGradient(rect, _listenedItemColor, dark); }
    inline QBrush & likedState(const QRect &  rect, bool dark) const { return buildGradient(rect, _likedItemColor, dark); }
    inline QBrush & playedState(const QRect &  rect, bool dark) const { return buildGradient(rect, _playedItemColor, dark); }
    inline QBrush & itemState(const QRect &  rect, bool dark) const { return buildGradient(rect, _itemColor, dark); }

    QBrush unprocessedState(const QRect & rect, bool dark);

    inline bool isUseGradient() const { return _useGradient; }
    inline void setUseGradient(bool use) { _useGradient = use; }

    const inline QColor & defaultItemColor() const { return _defaultItemColor; }
    inline void setDefaultItemColor(const QColor & newColor) { _defaultItemColor = newColor; }

    const inline QColor & listenedItemColor() const { return _listenedItemColor; }
    inline void setListenedItemColor(const QColor & newColor) { _listenedItemColor = newColor; }

    const inline QColor & likedItemColor() const { return _likedItemColor; }
    inline void setLikedItemColor(const QColor & newColor) { _likedItemColor = newColor; }

    const inline QColor & playedItemColor() const { return _playedItemColor; }
    inline void setPlayedItemColor(const QColor & newColor) { _playedItemColor = newColor; }

    const inline QColor & folderItemColor() const { return _folderItemColor; }
    inline void setFolderItemColor(const QColor & newColor) { _folderItemColor = newColor; }

    const inline QColor & itemColor() const { return _itemColor; }
    inline void setItemColor(const QColor & newColor) { _itemColor = newColor; }

    inline int itemHeight() const { return _itemHeight; }
    inline void setItemHeight(int newHeight) { _itemHeight = newHeight; }


    const inline QString & itemFontName() const { return __title.fontName; }
    inline void setItemFontName(const QString & newFontName) { __title.fontName = newFontName; }

    inline int itemFontSize() const { return __title.fontSize; }
    inline void setItemFontSize(int newFontSize) { __title.fontSize = newFontSize; }

    const inline QColor & itemTextColor() const { return __title.textColor; }
    inline void setItemTextColor(const QColor & newColor) { __title.textColor = newColor; }

    const inline QColor & selectedItemTextColor() const { return __title.selectedTextColor; }
    inline void setSelectedItemTextColor(const QColor & newColor) { __title.selectedTextColor = newColor; }


    const inline QString & itemInfoFontName() const { return __info.fontName; }
    inline void setItemInfoFontName(const QString & newFontName) { __info.fontName = newFontName; }

    inline int itemInfoFontSize() const { return __info.fontSize; }
    inline void setItemInfoFontSize(int newFontSize) { __info.fontSize = newFontSize; }

    const inline QColor & itemInfoTextColor() const { return __info.textColor; }
    inline void setItemInfoTextColor(const QColor & newColor) { __info.textColor = newColor; }

    const inline QColor & selectedItemInfoTextColor() const { return __info.selectedTextColor; }
    inline void setSelectedItemInfoTextColor(const QColor & newColor) { __info.selectedTextColor = newColor; }

    inline QFont itemFont() { return QFont(itemFontName(), itemFontSize(), QFont::Normal); }
    inline QFont itemInfoFont() { return QFont(itemInfoFontName(), itemInfoFontSize(), QFont::Bold); }
protected:
    bool _useGradient;
    int _itemHeight;
    QColor _defaultItemColor, _listenedItemColor, _likedItemColor, _playedItemColor, _folderItemColor, _itemColor;
    ItemTextAttrs __title, __info;
};

#endif // ITEM_SETTINGS
