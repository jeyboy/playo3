#ifndef LIBRARY_SETTINGS
#define LIBRARY_SETTINGS

#include "misc/web_utils/json.h"

#define SETTINGS_REMOTE_LIB_PROC_DELAY_KEY QStringLiteral("remote_lib_proc_delay")
#define SETTINGS_SAVE_LIB_DELAY_KEY QStringLiteral("save_lib_delay")
#define SETTINGS_INTERACTIVE_LIB_PROC_KEY QStringLiteral("interactive_lib_proc")
#define SETTINGS_SHOW_INFO_KEY QStringLiteral("show_info")

class LibrarySettings {
public:
    void fromJson(const Json::Obj & json = Json::Obj());
    void toJson(Json::Obj & json);

    inline bool isShowInfo() const { return _showInfo; }
    inline void setShowInfo(bool show) { _showInfo = show; }

    inline bool isInteractiveProc() const { return _interactiveProc; }
    inline void setInteractiveProc(bool is_interactive) { _interactiveProc = is_interactive; }

    inline bool isUsedDelayForRemote() const { return _remoteItemsProcDelay > 0; }

    inline int remoteItemsProcDelay() const { return _remoteItemsProcDelay; }
    inline void setRemoteItemsProcDelay(int new_delay) { _remoteItemsProcDelay = new_delay; }

    inline int saveLibDelay() const { return _saveLibDelay; }
    inline void setSaveLibDelay(int new_delay) { _saveLibDelay = new_delay; }
protected:
    int _remoteItemsProcDelay;
    int _saveLibDelay;

    bool _interactiveProc;
    bool _showInfo;
};

#endif // LIBRARY_SETTINGS
