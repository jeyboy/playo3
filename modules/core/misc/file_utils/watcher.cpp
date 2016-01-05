#include "watcher.h"

#ifdef Q_OS_WIN
    #include "watcher_win.cpp"
#elif Q_OS_MAC
    #include "watcher_mac.cpp"
#else
    #include "watcher_x11.cpp"
#endif

WatcherCell * Watcher::createCell(const QString & path, bool recursive) {
    #ifdef Q_OS_WIN
        return new WinWatcher(path, recursive);
    #elif Q_OS_MAC
        return new MacWatcher(path, recursive);
    #else
        return new X11Watcher(path, recursive);
    #endif
}
