#include "filesystem_watcher.h"

#include <sys/inotify.h>
#include <unistd.h>

#include <qhash.h>
#include <qthread.h>
#include <qdir.h>

namespace {
    #define BUFFERSIZE 512

    class INotifyThread : public QThread {
        qintptr notifyPID;
        QHash<uint32_t, QString> linked_operations;
        QHash<QString, bool> drives;
        QHash<QString, bool> media;
    protected:
        void run();
    public:
        INotifyThread(qintptr notifyPID) : notifyPID(notifyPID) {}
    };

    class X11Watcher : public Core::Singleton<X11Watcher> {
        qintptr notifyPID;

        INotifyThread * thread;
        QHash<qintptr, QList<qintptr> > recursiveTrees;

        bool initiated() { return notifyPID > -1; }
        public:
            X11Watcher() {
                notifyPID = inotify_init();
                if (!initiated()) return;
                thread = new INotifyThread(notifyPID);
                thread -> start();
            }
            ~X11Watcher() {
                if (!initiated()) return;

                close(notifyPID);
                thread -> terminate();
                thread -> wait(3000);
                delete thread;
            }

            bool registerPath(qintptr & id, const QString & path, bool /*recursive*/) {
                if (!initiated()) return false;

                //TODO: need to realize recursive schema

                id = inotify_add_watch(
                    notifyPID,
                    path.toUtf8().data(),
                    IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED
                );

                return id > -1;
            }
            void unregisterPath(qintptr ptr) {
                inotify_rm_watch(notifyPID, ptr);

                if (recursiveTrees.contains(ptr)) {
                    QList<qintptr> ptrs = recursiveTrees.take(ptr);
                    for(QList<qintptr>::Iterator subPtr = ptrs.begin(); subPtr != ptrs.end(); subPtr++)
                        inotify_rm_watch(notifyPID, *subPtr);
                }
            }
    };

    void INotifyThread::run() {
        uint len, i;
        struct inotify_event * event;
        char buffer[BUFFERSIZE];

        qDebug() << "IN NOTIFY THREAD";
        while((len = read(notifyPID, buffer, BUFFERSIZE)) > 0) {
            i = 0;

            while(i + sizeof(struct inotify_event) < len) {
                event = (struct inotify_event *) &buffer[i];

//                /* Events sent by the kernel.  */
//                #define IN_UNMOUNT	 0x00002000	/* Backing fs was unmounted.  */
//                #define IN_Q_OVERFLOW	 0x00004000	/* Event queued overflowed.  */
//                #define IN_IGNORED	 0x00008000	/* File was ignored.  */

                QString n1(event -> name);

                switch (event -> mask) {
                    case IN_ATTRIB: {
                        qDebug() << QString("Got change ATTRIBUTES for %1.").arg(n1);
                        emit FileSystemWatcher::obj().attributeChanged(n1);
                    break;}

                    case IN_OPEN: {
                        qDebug() << QString("OPEN for %1.").arg(n1);
                        emit FileSystemWatcher::obj().fileOpened(n1);
                    break;}

                    case IN_ACCESS: {
                        qDebug() << QString("READ for %1.").arg(n1);
                        emit FileSystemWatcher::obj().fileReaded(n1);
                    break;}

                    case IN_CLOSE_WRITE:
                    case IN_CLOSE_NOWRITE: {
                        qDebug() << QString("CLOSE for %1.").arg(n1);
                        emit FileSystemWatcher::obj().fileClosed(n1);
                    break;}

                    case IN_CREATE: {
                        qDebug() << QString("Got change CREATE for %1.").arg(n1);
                        emit FileSystemWatcher::obj().fileCreated(n1);
                    break;}

                    case IN_DELETE_SELF:
                    case IN_DELETE: {
                        qDebug() << QString("Got change DELETE %1.").arg(n1);
                        emit FileSystemWatcher::obj().fileDeleted(n1);
                    break;}


                    case IN_ISDIR | IN_MOVE_SELF:
                    case IN_ISDIR | IN_MOVED_FROM:
                    case IN_MOVE_SELF:
                    case IN_MOVED_FROM: {
                        linked_operations.insert(event -> cookie, n1);
                    break;}

                    case IN_MOVED_TO: {
                        QString n0 = linked_operations.take(event -> cookie);
                        qDebug() << QString("Got change RENAMEITEM %1 to %2.").arg(n0).arg(n1);
                        emit FileSystemWatcher::obj().fileRenamed(n0, n1);
                    break;}

                    case IN_MODIFY: {
                       qDebug() << QString("Got change UPDATEITEM %1.").arg(n1);
                       emit FileSystemWatcher::obj().fileChanged(n1);
                    break;}

                    case IN_ISDIR | IN_MODIFY:
                    case IN_ISDIR | IN_ATTRIB: {
                        qDebug() << QString("Got change UPDATEDIR %1.").arg(n1);
                        emit FileSystemWatcher::obj().folderChanged(n1);
                    break;}

                    case IN_ISDIR | IN_CREATE: {
                        qDebug() << QString("Got change MKDIR %1.").arg(n1);
                        emit FileSystemWatcher::obj().folderCreated(n1);
                    break;}

                    case IN_ISDIR | IN_DELETE_SELF:
                    case IN_ISDIR | IN_DELETE: {
                        qDebug() << QString("Got change RMDIR %1.").arg(n1);
                        emit FileSystemWatcher::obj().folderDeleted(n1);
                    break;}

                    case IN_ISDIR | IN_MOVED_TO: {
                        QString n0 = linked_operations.take(event -> cookie);
                        qDebug() << QString("Got change RENAMEFOLDER %1 to %2.").arg(n0).arg(n1);
                        emit FileSystemWatcher::obj().folderRenamed(n0, n1);
                    break;}


                    case IN_IGNORED:
                    case IN_UNMOUNT: {
                        qDebug() << QString("UNMOUNT");

//                        unregisterPath();
                    break;}

                    case IN_Q_OVERFLOW: {
                        qDebug() << QString("OVERFLOW!!!");
                    break;}


//                    /media,/mnt
                    //                    case SHCNE_DRIVEADD: {
                    //                       qDebug() << QString("Got change DRIVEADD %1.").arg(n1);
                    //                       emit FileSystemWatcher::obj().driveAdded(n1);
                    //                    break;}
                    //                    case SHCNE_DRIVEADDGUI: {
                    //                       qDebug() << QString("Got change DRIVEADDGUI %1.").arg(n1);
                    //                       emit FileSystemWatcher::obj().driveGuiAdded(n1);
                    //                    break;}
                    //                    case SHCNE_DRIVEREMOVED: {
                    //                       qDebug() << QString("Got change DRIVEREMOVED %1.").arg(n1);
                    //                       emit FileSystemWatcher::obj().driveRemoved(n1);
                    //                    break;}

                    //                    case SHCNE_MEDIAINSERTED: {
                    //                       qDebug() << QString("Got change MEDIAINSERTED %1.").arg(n1);
                    //                       emit FileSystemWatcher::obj().mediaInserted(n1);
                    //                    break;}
                    //                    case SHCNE_MEDIAREMOVED: {
                    //                       qDebug() << QString("Got change MEDIAREMOVED %1.").arg(n1);
                    //                       emit FileSystemWatcher::obj().mediaRemoved(n1);
                    //                    break;}
                }

                i += sizeof(struct inotify_event) + event -> len;
            }
        }

        qDebug() << "OUT NOTIFY THREAD";
    }
}

