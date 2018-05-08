#ifndef ITEM_SETTINGS
#define ITEM_SETTINGS

#include <qvariant.h>
#include <qfont.h>
#include <qbrush.h>
#include <qjsonobject.h>

#define SETTINGS_OPEN_TIMEOUT_KEY QLatin1String("open_time_out_key")
#define SETTINGS_USE_GRADIENT_KEY QLatin1String("use_gradient")
#define SETTINGS_ITEM_HEIGHT_KEY QLatin1String("item_height")
#define SETTINGS_DEFAULT_ITEM_COLOR_KEY QLatin1String("default_color")
#define SETTINGS_LISTENED_ITEM_COLOR_KEY QLatin1String("listened_color")
#define SETTINGS_LIKED_ITEM_COLOR_KEY QLatin1String("liked_color")
#define SETTINGS_PLAYED_ITEM_COLOR_KEY QLatin1String("played_color")
#define SETTINGS_FOLDER_ITEM_COLOR_KEY QLatin1String("folder_color")
#define SETTINGS_ITEM_COLOR_KEY QLatin1String("item_color")
#define SETTINGS_ITEM_FONT_NAME_KEY QLatin1String("item_font_name")
#define SETTINGS_ITEM_FONT_SIZE_KEY QLatin1String("item_font_size")
#define SETTINGS_ITEM_TEXT_COLOR_KEY QLatin1String("item_text_color")
#define SETTINGS_SELECTED_ITEM_TEXT_COLOR QLatin1String("selected_item_text_color")
#define SETTINGS_ITEM_INFO_FONT_NAME_KEY QLatin1String("item_info_font_name")
#define SETTINGS_ITEM_INFO_FONT_SIZE_KEY QLatin1String("item_info_font_size")
#define SETTINGS_ITEM_INFO_TEXT_COLOR_KEY QLatin1String("item_info_text_color")
#define SETTINGS_SELECTED_ITEM_INFO_TEXT_COLOR_KEY QLatin1String("selected_item_info_text_color")

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

    inline QBrush defaultState(const QRect & rect, bool dark) { return buildGradient(rect, _default_item_color, dark); }
    inline QBrush listenedState(const QRect & rect, bool dark) { return buildGradient(rect, _listened_item_color, dark); }
    inline QBrush likedState(const QRect & rect, bool dark) { return buildGradient(rect, _liked_item_color, dark); }
    inline QBrush playedState(const QRect & rect, bool dark) { return buildGradient(rect, _played_item_color, dark); }
    inline QBrush itemState(const QRect & rect, bool dark) { return buildGradient(rect, _item_color, dark); }

    QBrush unprocessedState(const QRect & rect, bool dark);

    inline bool isUseGradient() const { return _use_gradient; }
    inline void setUseGradient(const bool & use) { _use_gradient = use; }

    inline QColor defaultItemColor() const { return _default_item_color; }
    inline void setDefaultItemColor(const QColor & new_color) { _default_item_color = new_color; }

    inline QColor listenedItemColor() const { return _listened_item_color; }
    inline void setListenedItemColor(const QColor & new_color) { _listened_item_color = new_color; }

    inline QColor likedItemColor() const { return _liked_item_color; }
    inline void setLikedItemColor(const QColor & new_color) { _liked_item_color = new_color; }

    inline QColor playedItemColor() const { return _played_item_color; }
    inline void setPlayedItemColor(const QColor & new_color) { _played_item_color = new_color; }

    inline QColor folderItemColor() const { return _folder_item_color; }
    inline void setFolderItemColor(const QColor & new_color) { _folder_item_color = new_color; }

    inline QColor itemColor() const { return _item_color; }
    inline void setItemColor(const QColor & new_color) { _item_color = new_color; }

    inline int itemHeight() { return _item_height; }
    inline void setItemHeight(const int & new_height) { _item_height = new_height; }


    inline QString itemFontName() { return __title.fontName; }
    inline void setItemFontName(const QString & new_font_name) { __title.fontName = new_font_name; }

    inline int itemFontSize() { return __title.fontSize; }
    inline void setItemFontSize(const int & new_font_size) { __title.fontSize = new_font_size; }

    inline QColor itemTextColor() { return __title.textColor; }
    inline void setItemTextColor(const QColor & new_color) { __title.textColor = new_color; }

    inline QColor selectedItemTextColor() { return __title.selectedTextColor; }
    inline void setSelectedItemTextColor(const QColor & new_color) { __title.selectedTextColor = new_color; }


    inline QString itemInfoFontName() { return __info.fontName; }
    inline void setItemInfoFontName(const QString & new_font_name) { __info.fontName = new_font_name; }

    inline int itemInfoFontSize() { return __info.fontSize; }
    inline void setItemInfoFontSize(const int & new_font_size) { __info.fontSize = new_font_size; }

    inline QColor itemInfoTextColor() { return __info.textColor; }
    inline void setItemInfoTextColor(const QColor & new_color) { __info.textColor = new_color; }

    inline QColor selectedItemInfoTextColor() { return __info.selectedTextColor; }
    inline void setSelectedItemInfoTextColor(const QColor & new_color) { __info.selectedTextColor = new_color; }

    inline QFont itemFont() { return QFont(itemFontName(), itemFontSize(), QFont::Normal); }
    inline QFont itemInfoFont() { return QFont(itemInfoFontName(), itemInfoFontSize(), QFont::Bold); }

    inline float openTimeOut() { return _open_time_out; }
    inline void setOpenTimeOut(int new_time_out) { _open_time_out = new_time_out; }
protected:
    bool _use_gradient;
    int _item_height;
    QColor _default_item_color, _listened_item_color, _liked_item_color, _played_item_color, _folder_item_color, _item_color;
    ItemTextAttrs __title, __info;
    float _open_time_out;
};

#endif // ITEM_SETTINGS
