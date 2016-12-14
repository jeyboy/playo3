#ifndef LIBRARY_SETTINGS
#define LIBRARY_SETTINGS

#include <qjsonobject.h>

#define SETTINGS_REMOTE_LIB_PROC_DELAY_KEY QStringLiteral("remote_lib_proc_delay")
#define SETTINGS_SAVE_LIB_DELAY_KEY QStringLiteral("save_lib_delay")
#define SETTINGS_INITIATE_ON_PLAYING_KEY QStringLiteral("initiate_on_playing")
#define SETTINGS_SHOW_INFO_KEY QStringLiteral("show_info")

class LibrarySettings {
public:
    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isShowInfo() const { return _show_info; }
    inline void setShowInfo(bool show) { _show_info = show; }

    inline bool isInitiateOnPlaying() const { return _initiate_on_play; }
    inline void setInitiateOnPlaying(bool on_play) { _initiate_on_play = on_play; }

    inline bool isUsedDelayForRemote() const { return _remote_items_proc_delay > 0; }

    inline int remoteItemsProcDelay() const { return _remote_items_proc_delay; }
    inline void setRemoteItemsProcDelay(int new_delay) { _remote_items_proc_delay = new_delay; }

    inline int saveLibDelay() const { return _save_lib_delay; }
    inline void setSaveLibDelay(int new_delay) { _save_lib_delay = new_delay; }
protected:
    int _remote_items_proc_delay;
    int _save_lib_delay;

    bool _initiate_on_play;
    bool _show_info;
};

#endif // LIBRARY_SETTINGS
