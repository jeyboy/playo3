#ifndef VIEW_SETTINGS
#define VIEW_SETTINGS

#include <qjsonobject.h>

class ViewSettings {
public:
    ViewSettings::ViewSettings(QJsonObject & settings);

    inline bool isShowSystemIcons() const { return _showSystemIcons; }
    inline void setShowSystemIcons(bool show) { _showSystemIcons = show; }

    inline bool isShowInfo() const { return _showInfo; }
    inline void setShowInfo(bool show) { _showInfo = show; }

    inline bool isSpoilOnActivation() const { return _spoilOnActivation; }
    inline void setSpoilOnActivation(bool show) { _spoilOnActivation = show; }

    inline bool isAlertOnFolderDeletion() const { return _alertOnFolderDeletion; }
    inline void setAlertOnFolderDeletion(bool alert) { _alertOnFolderDeletion = alert; }

    inline int folderDeletionAnswer() const { return _deleteFolderAnswer; }
    inline void setfolderDeletionAnswer(int ans) { _deleteFolderAnswer = ans; }

    inline bool isCheckboxShow() const { return _showCheckbox; }
    inline void setCheckboxShow(bool show) { _showCheckbox = show; }

    inline int tabPosition() { return _tabPosition; }
    inline void setTabPosition(int newPositionsType) { _tabPosition = newPositionsType; }
protected:
    int _tabPosition;
    int _treeIndentation;

    bool _showSystemIcons;
    bool _showInfo;
    bool _showCheckbox;
    bool _spoilOnActivation;
    bool _alertOnFolderDeletion;

    ///////// session settings ///////////
    int _deleteFolderAnswer;
};

#endif // VIEW_SETTINGS
