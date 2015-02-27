#ifndef ITEM_SETTINGS
#define ITEM_SETTINGS

#include <QFont>
#include <QLinearGradient>

struct ItemTextAttrs {
    QString fontName;
    int fontSize;
    QColor textColor;
    QColor selectedTextColor;
};

class ItemSettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    QBrush buildGradient(QRect rect, QColor color, bool dark);

    inline QBrush defaultState(QRect rect, bool dark) { return buildGradient(rect, _defaultItemColor, dark); }
    inline QBrush listenedState(QRect rect, bool dark) { return buildGradient(rect, _listenedItemColor, dark); }
    inline QBrush likedState(QRect rect, bool dark) { return buildGradient(rect, _likedItemColor, dark); }
    inline QBrush playedState(QRect rect, bool dark) { return buildGradient(rect, _playedItemColor, dark); }

    QBrush unprocessedState(QRect rect, bool dark);

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
protected:
    bool _useGradient;
    int _itemHeight;
    QColor _defaultItemColor, _listenedItemColor, _likedItemColor, _playedItemColor, _folderItemColor;
    ItemTextAttrs __title, __info;
};

#endif // ITEM_SETTINGS
