#ifndef TAB_SETTINGS
#define TAB_SETTINGS

#include <qjsonobject.h>

#define SETTINGS_SHOW_CHECKBOX_KEY QStringLiteral("show_checkboxes")
#define SETTINGS_SPOIL_ON_ACTIVATION_KEY QStringLiteral("spoil_on_activation")
#define SETTINGS_ALERT_ON_FOLDER_DELETION_KEY QStringLiteral("alert_on_folder_deletion")
#define SETTINGS_SHOW_SYSTEM_ICON_KEY QStringLiteral("show_system_icons")
#define SETTINGS_HEIGHT_UNIFICATION_KEY QStringLiteral("height_unification")
#define SETTINGS_TREE_IDENTATION_KEY QStringLiteral("tree_indentation")
#define SETTINGS_ITEM_PRESENTATION_TYPE_KEY QStringLiteral("item_present_type")
#define SETTINGS_FIND_VALID_KEY QStringLiteral("find_valid")

class TabSettings {
public:
    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isShowSystemIcons() const { return _showSystemIcons; }
    inline void setShowSystemIcons(bool show) { _showSystemIcons = show; }

    inline bool isSpoilOnActivation() const { return _spoilOnActivation; }
    inline void setSpoilOnActivation(bool show) { _spoilOnActivation = show; }

    inline bool isAlertOnFolderDeletion() const { return _alertOnFolderDeletion; }
    inline void setAlertOnFolderDeletion(bool alert) { _alertOnFolderDeletion = alert; }

    inline bool isCheckboxShow() const { return _showCheckbox; }
    inline void setCheckboxShow(bool show) { _showCheckbox = show; }

    inline bool isHeightUnificate() const { return _heightUnification; }
    inline void setHeightUnification(bool unify) { _heightUnification = unify; }

    inline int treeIndentation() const { return _treeIndentation; }
    inline void setTreeIndentation(int newIndent) { _treeIndentation = newIndent; }

    inline int itemPresentType() const { return _item_present_type; }
    inline void setItemPresentType(int newType) { _item_present_type = newType; }

    inline bool isFindValid() const { return _findValidOnFailure; }
    inline void setFindValid(bool find) { _findValidOnFailure = find; }
protected:
    int _treeIndentation;
    int _item_present_type;

    bool _heightUnification;
    bool _showSystemIcons;
    bool _showCheckbox;
    bool _spoilOnActivation;
    bool _alertOnFolderDeletion;
    bool _findValidOnFailure;
};

#endif // TAB_SETTINGS
