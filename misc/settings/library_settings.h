#ifndef LIBRARY_SETTINGS
#define LIBRARY_SETTINGS

#include <qjsonobject.h>

class LibrarySettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isShowInfo() const { return _showInfo; }
    inline void setShowInfo(bool show) { _showInfo = show; }

    inline bool isInteractiveProc() const { return _interactiveProc; }
    inline void setInteractiveProc(bool is_interactive) { _interactiveProc = is_interactive; }

    inline bool isUsedDelayForRemote() const { return _usedDelayForRemote; }
    inline void setUsedDelayForRemote(bool is_use_delay) { _usedDelayForRemote = is_use_delay; }

    inline int remoteItemsProcDelay() const { return _remoteItemsProcDelay; }
    inline void setRemoteItemsProcDelay(int new_delay) { _remoteItemsProcDelay = new_delay; }

    inline int saveLibDelay() const { return _saveLibDelay; }
    inline void setSaveLibDelay(int new_delay) { _saveLibDelay = new_delay; }
protected:
    int _remoteItemsProcDelay;
    int _saveLibDelay;

    bool _interactiveProc;
    bool _usedDelayForRemote;
    bool _showInfo;
};

#endif // LIBRARY_SETTINGS
