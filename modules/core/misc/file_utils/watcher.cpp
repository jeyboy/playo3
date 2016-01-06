#include "watcher.h"

#ifdef Q_OS_WIN
    #include "watcher_win.cpp"
#elif Q_OS_MAC
    #include "watcher_mac.cpp"
#else
    #include "watcher_x11.cpp"
#endif

bool Watcher::registerPathProc(qintptr & ptr, const QString & path, bool recursive) {
    #ifdef Q_OS_WIN
        return WinWatcher::obj().registerPath(ptr, path, recursive);
    #elif Q_OS_MAC
        return MacWatcher::obj().registerPath(ptr, path, recursive);
    #else
        return X11Watcher::obj().registerPath(ptr, path, recursive);
    #endif
}
void Watcher::unregisterPathProc(qintptr ptr) {
    #ifdef Q_OS_WIN
        WinWatcher::obj().unregisterPath(ptr);
    #elif Q_OS_MAC
        MacWatcher::obj().unregisterPath(ptr);
    #else
        X11Watcher::obj().unregisterPath(ptr);
    #endif
}
