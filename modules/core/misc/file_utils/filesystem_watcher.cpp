#include "filesystem_watcher.h"

#ifdef Q_OS_WIN
    #include "watcher_win.cpp"
#elif Q_OS_MAC
    #include "watcher_mac.cpp"
#else
    #include "watcher_x11.cpp"
#endif

bool FileSystemWatcher::registerPathProc(qintptr & ptr, const QString & path, bool recursive) {
    #ifdef Q_OS_WIN
        return WinWatcher::obj().registerPath(ptr, path, recursive);
    #elif Q_OS_MAC
        return MacWatcher::obj().registerPath(ptr, path, recursive);
    #else
        return X11Watcher::obj().registerPath(ptr, path, recursive);
    #endif
}
void FileSystemWatcher::unregisterPathProc(qintptr ptr) {
    #ifdef Q_OS_WIN
        WinWatcher::obj().unregisterPath(ptr);
    #elif Q_OS_MAC
        MacWatcher::obj().unregisterPath(ptr);
    #else
        X11Watcher::obj().unregisterPath(ptr);
    #endif
}

FileSystemWatcher::~FileSystemWatcher() {
    clear();

    #ifdef Q_OS_WIN
        WinWatcher::destroy();
    #elif Q_OS_MAC
        MacWatcher::destroy();
    #else
        X11Watcher::destroy();
    #endif
}
