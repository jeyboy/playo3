#ifndef TAB_SETTINGS
#define TAB_SETTINGS

#include <qjsonobject.h>

#define SETTINGS_SHOW_CHECKBOX_KEY QLatin1String("show_checkboxes")
#define SETTINGS_SPOIL_ON_ACTIVATION_KEY QLatin1String("spoil_on_activation")
#define SETTINGS_ALERT_ON_FOLDER_DELETION_KEY QLatin1String("alert_on_folder_deletion")
#define SETTINGS_SHOW_SYSTEM_ICON_KEY QLatin1String("show_system_icons")
#define SETTINGS_HEIGHT_UNIFICATION_KEY QLatin1String("height_unification")
#define SETTINGS_TREE_IDENTATION_KEY QLatin1String("tree_indentation")
#define SETTINGS_ITEM_PRESENTATION_TYPE_KEY QLatin1String("item_present_type")
#define SETTINGS_FIND_VALID_KEY QLatin1String("find_valid")
#define SETTINGS_SHOW_NUMBER_KEY QLatin1String("show_number")

class TabSettings {
public:
    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isShowSystemIcons() const { return _show_system_icons; }
    inline void setShowSystemIcons(const bool & show) { _show_system_icons = show; }

    inline bool isSpoilOnActivation() const { return _spoil_on_activation; }
    inline void setSpoilOnActivation(const bool & show) { _spoil_on_activation = show; }

    inline bool isAlertOnFolderDeletion() const { return _alert_on_folder_deletion; }
    inline void setAlertOnFolderDeletion(const bool & alert) { _alert_on_folder_deletion = alert; }

    inline bool isCheckboxShow() const { return _show_checkbox; }
    inline void setCheckboxShow(const bool & show) { _show_checkbox = show; }

    inline bool isHeightUnificate() const { return _height_unification; }
    inline void setHeightUnification(const bool & unify) { _height_unification = unify; }

    inline int treeIndentation() const { return _tree_indentation; }
    inline void setTreeIndentation(const int & new_indent) { _tree_indentation = new_indent; }

    inline int itemPresentType() const { return _item_present_type; }
    inline void setItemPresentType(const int & new_type) { _item_present_type = new_type; }

    inline bool isFindValid() const { return _find_valid_on_failure; }
    inline void setFindValid(const bool & find) { _find_valid_on_failure = find; }

    inline bool isShowNumber() const { return _show_number; }
    inline void setShowNumber(const bool & show) { _show_number = show; }
protected:
    int _tree_indentation;
    int _item_present_type;

    bool _height_unification;
    bool _show_system_icons;
    bool _show_checkbox;
    bool _spoil_on_activation;
    bool _alert_on_folder_deletion;
    bool _find_valid_on_failure;
    bool _show_number;
};

#endif // TAB_SETTINGS
