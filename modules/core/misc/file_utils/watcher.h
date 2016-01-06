#ifndef FILE_WATCHER
#define FILE_WATCHER

#include <qobject.h>
#include <qdir.h>
#include <qdebug.h>

#include "modules/core/interfaces/singleton.h"

class Watcher : public QWidget, public Core::Singleton<Watcher> {
    Q_OBJECT

    QHash<QString, qintptr> pathPids;
protected:
    bool registerPathProc(qintptr & ptr, const QString & path, bool recursive = false);
    void unregisterPathProc(qintptr ptr);
public:
    virtual ~Watcher() {
        for(QHash<QString, qintptr>::Iterator pid = pathPids.begin(); pid != pathPids.end(); pid++)
            unregisterPathProc(pid.value());

        pathPids.clear();
    }
    bool registerPath(const QString & path, bool recursive = false) {
        qintptr ptr;
        bool res = registerPathProc(ptr, path, recursive);

        if (res)
            pathPids.insert(path, ptr);

        return res;
    }
    void unregisterPath(const QString & path) {
        if (pathPids.contains(path))
            unregisterPathProc(pathPids.take(path));
    }

signals:
    void freeSpaceChanged(const QString & path);

    void attributeChanged(const QString & path);

    // x11 only
    void fileOpened(const QString & path);
    void fileReaded(const QString & path);
    void fileClosed(const QString & path);
    ////////////////////////

    void fileCreated(const QString & path);
    void fileDeleted(const QString & path);
    void fileRenamed(const QString & oldPath, const QString & newPath);
    void fileChanged(const QString & path);

    void driveAdded(const QString & path);
    void driveRemoved(const QString & path);

    void mediaInserted(const QString & path);
    void mediaRemoved(const QString & path);

    void folderCreated(const QString & path);
    void folderDeleted(const QString & path);
    void folderRenamed(const QString & oldPath, const QString & newPath);
    void folderChanged(const QString & path);
};

#endif // FILE_WATCHER
