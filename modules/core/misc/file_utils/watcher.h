#ifndef WATCHER
#define WATCHER

#include <qobject.h>

#include "modules/core/interfaces/singleton.h"

class WatcherCell {
public:
    virtual ~WatcherCell() {}
};

class Watcher : public QObject, public Core::Singleton<Watcher> {
    Q_OBJECT

    QHash<QString, WatcherCell *> watchers;
protected:
    WatcherCell * createCell(const QString & path, bool recursive);
public:
    bool registerPath(const QString & path, bool recursive = false) {
        WatcherCell * cell = createCell(path, recursive);
        if (cell)
            watchers.insert(path, cell);

        return !!cell;
    }
    void unregisterPath(const QString & path) {
        delete watchers.take(path);
    }

    ~Watcher() {
        qDeleteAll(watchers);
    }
signals:
    void attributeChanged(const QString & path);
    void fileCreated(const QString & path);
    void fileDeleted(const QString & path);
    void fileRenamed(const QString & oldPath, const QString & newPath);
    void fileChanged(const QString & path);

    void driveAdded(const QString & path);
    void driveGuiAdded(const QString & path);
    void driveRemoved(const QString & path);

    void mediaInserted(const QString & path);
    void mediaRemoved(const QString & path);

    void folderCreated(const QString & path);
    void folderDeleted(const QString & path);
    void folderRenamed(const QString & oldPath, const QString & newPath);
    void folderChanged(const QString & path);
};

#endif // WATCHER
