#ifndef LIBRARY_SETTINGS
#define LIBRARY_SETTINGS

#include <qjsonobject.h>

class LibrarySettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline bool isInteractiveProc() const { return _interactiveProc; }
    inline void setInteractiveProc(bool is_interactive) { _interactiveProc = is_interactive; }

    inline int remoteItemsProcDelay() const { return _remoteItemsProcDelay; }
    inline void setRemoteItemsProcDelay(int new_delay) { _remoteItemsProcDelay = new_delay; }

    inline int saveLibDelay() const { return _saveLibDelay; }
    inline void setSaveLibDelay(int new_delay) { _saveLibDelay = new_delay; }
protected:
    int _remoteItemsProcDelay;
    int _saveLibDelay;

    bool _interactiveProc;
};

#endif // LIBRARY_SETTINGS
