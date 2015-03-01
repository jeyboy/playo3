#ifndef TAB_SETTINGS
#define TAB_SETTINGS

#include <qjsonobject.h>

class TabSettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isShowSystemIcons() const { return _showSystemIcons; }
    inline void setShowSystemIcons(bool show) { _showSystemIcons = show; }

    inline bool isShowInfo() const { return _showInfo; }
    inline void setShowInfo(bool show) { _showInfo = show; }

    inline bool isSpoilOnActivation() const { return _spoilOnActivation; }
    inline void setSpoilOnActivation(bool show) { _spoilOnActivation = show; }

    inline bool isAlertOnFolderDeletion() const { return _alertOnFolderDeletion; }
    inline void setAlertOnFolderDeletion(bool alert) { _alertOnFolderDeletion = alert; }

    inline bool isCheckboxShow() const { return _showCheckbox; }
    inline void setCheckboxShow(bool show) { _showCheckbox = show; }

    inline int treeIndentation() const { return _treeIndentation; }
    inline void setTreeIndentation(int newIndent) { _treeIndentation = newIndent; }
protected:
    int _treeIndentation;

    bool _showSystemIcons;
    bool _showInfo;
    bool _showCheckbox;
    bool _spoilOnActivation;
    bool _alertOnFolderDeletion;
};

#endif // TAB_SETTINGS