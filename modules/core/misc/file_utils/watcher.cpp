#include "watcher.h"

#ifdef Q_OS_WIN
    #include "watcher_win.cpp"
#elif Q_OS_MAC

#else

#endif

WatcherCell * Watcher::createCell(const QString & path, bool recursive) {
    #ifdef Q_OS_WIN
        return new WinWatcher(path, recursive);
    #elif Q_OS_MAC
        return 0;
    #else
        return 0;
    #endif
}
