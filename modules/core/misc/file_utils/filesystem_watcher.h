#ifndef FILE_SYSTEM_WATCHER
#define FILE_SYSTEM_WATCHER

#include <qobject.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <qdebug.h>

#include "modules/core/interfaces/singleton.h"

//QDir::NoSymLinks
#define COMMON_FILTERS ((int)QDir::NoDotAndDotDot | (int)QDir::Hidden | (int)QDir::System)
#define FILE_FILTERS ((int)QDir::Files | COMMON_FILTERS)
#define FOLDER_FILTERS ((int)QDir::Drives | (int)QDir::Dirs | COMMON_FILTERS)

class FileSystemWatcher : public QWidget, public Core::Singleton<FileSystemWatcher> {
    Q_OBJECT

    QHash<QString, qintptr> pathPids;
protected:
    bool registerPathProc(qintptr & ptr, const QString & path, bool recursive = false);
    void unregisterPathProc(qintptr ptr);

    static void entriesList(const QString & path, QStringList & result, int filters, const QStringList & nameFilters) {
        result = nameFilters.isEmpty() ?
                QDir(path).entryList((QDir::Filter)filters) :
                QDir(path).entryList(nameFilters, (QDir::Filter)filters);
    }

    static void entriesInfoList(const QFileInfo & file, QFileInfoList & result, int filters, const QStringList & nameFilters) {
        result = nameFilters.isEmpty() ?
                QDir(file.filePath()).entryInfoList((QDir::Filter)filters) :
                QDir(file.filePath()).entryInfoList(nameFilters, (QDir::Filter)filters);
    }
public:
    static void filesList(const QString & path, QStringList & result, const QStringList & nameFilters = QStringList()) {
        entriesList(path, result, FILE_FILTERS, nameFilters);
    }

    static void foldersList(const QString & path, QStringList & result, const QStringList & nameFilters = QStringList()) {
        entriesList(path, result, FOLDER_FILTERS, nameFilters);
    }

    static void filesList(const QFileInfo & file, QFileInfoList & result, const QStringList & nameFilters = QStringList()) {
        entriesInfoList(file, result, FILE_FILTERS, nameFilters);
    }

    static void foldersList(const QFileInfo & file, QFileInfoList & result, const QStringList & nameFilters = QStringList()) {
        entriesInfoList(file, result, FOLDER_FILTERS, nameFilters);
    }

    virtual ~FileSystemWatcher();

    bool registerPath(const QString & path, bool recursive = false) {
        qintptr ptr;
        bool res = registerPathProc(ptr, path, recursive);
        qDebug() << "REG" << path << res;

        if (res)
            pathPids.insert(path, ptr);

        return res;
    }
    void unregisterPath(const QString & path) {
        if (pathPids.contains(path))
            unregisterPathProc(pathPids.take(path));
    }

    void clear() {
        for(QHash<QString, qintptr>::Iterator pid = pathPids.begin(); pid != pathPids.end(); pid++)
            unregisterPathProc(pid.value());

        pathPids.clear();
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

#endif // FILE_SYSTEM_WATCHER
