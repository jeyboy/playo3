#include <shlobj.h>
#include <shlwapi.h>
//#include <qlibrary.h>
#include <qwidget.h>

#include "filesystem_watcher.h"

namespace {
    #define msgShellChange WM_USER + 1

    class WinWatcher : public QWidget, public Core::RelSingleton<WinWatcher> {
        static QString getPidlPath(ITEMIDLIST * pidl);
        public:
            WinWatcher() {}
            ~WinWatcher() {}

            bool registerPath(qintptr & id, const QString & path, bool recursive);
            void unregisterPath(qintptr ptr);
        protected:
            bool nativeEvent(const QByteArray & eventType, void * message, long * result);
    };

    bool WinWatcher::registerPath(qintptr & id, const QString & path, bool recursive) {
        ITEMIDLIST * dir = ILCreateFromPathW(path.toStdWString().c_str()); // this requires on test
//        qDebug() << "~~~~~~~~~~~~~" << WinWatcher::getPidlPath(dir);

        SHChangeNotifyEntry ne = {dir, recursive};

        id = SHChangeNotifyRegister((HWND)winId(), SHCNRF_InterruptLevel | SHCNRF_ShellLevel | SHCNRF_RecursiveInterrupt | SHCNRF_NewDelivery,
//                                    SHCNE_NETSHARE
//                                    SHCNE_NETUNSHARE
//                                    SHCNE_SERVERDISCONNECT
                                    SHCNE_FREESPACE        |
                                    SHCNE_ATTRIBUTES       |
                                    SHCNE_CREATE           |
                                    SHCNE_DELETE           |
                                    SHCNE_DRIVEADD         |
                                    SHCNE_DRIVEREMOVED     |
                                    SHCNE_MEDIAINSERTED    |
                                    SHCNE_MEDIAREMOVED     |
                                    SHCNE_MKDIR            |
                                    SHCNE_RENAMEFOLDER     |
                                    SHCNE_RENAMEITEM       |
                                    SHCNE_RMDIR            |
                                    SHCNE_UPDATEDIR        |
                                    SHCNE_UPDATEITEM,
                                    msgShellChange, 1, &ne);
        qDebug() << "SHChangeNotifyRegister id =" << id;
        ILFree(dir);
        return id > 0;
    }

    void WinWatcher::unregisterPath(qintptr ptr) {
        if(ptr) SHChangeNotifyDeregister(ptr);
    }

    QString WinWatcher::getPidlPath(ITEMIDLIST * pidl) {
        if (!pidl) return "";

        QString name;
        wchar_t str[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, str))
            name = QString::fromWCharArray(str);

        return name;

//        SHFILEINFO shfi;
//        QString name =
//           (SHGetFileInfo((WCHAR *) pidl, 0, &shfi, sizeof(shfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
//           ? QString::fromWCharArray(shfi.szDisplayName) : "<unknown>";
//        ITEMIDLIST * p = pidl;
//        if (p -> mkid.cb) {
//            for (;;) {
//                ITEMIDLIST * q = (ITEMIDLIST *) (((char *) p) + p -> mkid.cb);
//                if (q -> mkid.cb == 0) break;
//                p = q;
//            }

//            int n = (char *)p - (char *)pidl;
//            char * s = new char[n + 2];
//            memcpy(s, pidl, n);
//            s[n] = s[n + 1] = 0;
//            name = getPidlPath((ITEMIDLIST *) s) + "\\" + name;
//        }
//        return name;
    }

    bool WinWatcher::nativeEvent(const QByteArray & eventType, void * message, long * /*result*/) {
//        Q_NOT_USED(result);

        if (eventType != "windows_generic_MSG") return false;
        MSG * msg = (MSG *)message;
        if (msg -> message != msgShellChange) return false;

        long event;
        ITEMIDLIST ** items;
        HANDLE lock = SHChangeNotification_Lock((HANDLE) msg -> wParam, msg -> lParam, &items, &event);
        QString n1 = QDir::fromNativeSeparators(getPidlPath(items[0]));
        QString n2 = QDir::fromNativeSeparators(getPidlPath(items[1]));

        switch (event) {
            case SHCNE_FREESPACE: {
               qDebug() << QString("Got change FREE SPACE for %1.").arg(n1);
               emit FileSystemWatcher::obj().freeSpaceChanged(n1);
            break;}

            case SHCNE_ATTRIBUTES: {
               qDebug() << QString("Got change ATTRIBUTES for %1.").arg(n1);
               emit FileSystemWatcher::obj().attributeChanged(n1);
            break;}

            case SHCNE_CREATE: {
               qDebug() << QString("Got change CREATE for %1.").arg(n1);
               emit FileSystemWatcher::obj().fileCreated(n1);
            break;}
            case SHCNE_DELETE: {
               qDebug() << QString("Got change DELETE %1.").arg(n1);
               emit FileSystemWatcher::obj().fileDeleted(n1);
            break;}
            case SHCNE_RENAMEITEM: {
               qDebug() << QString("Got change RENAMEITEM %1 to %2.").arg(n1).arg(n2);
               emit FileSystemWatcher::obj().fileRenamed(n1, n2);
            break;}
            case SHCNE_UPDATEITEM: {
               qDebug() << QString("Got change UPDATEITEM %1.").arg(n1);
               emit FileSystemWatcher::obj().fileChanged(n1);
            break;}

            case SHCNE_DRIVEADD: {
               qDebug() << QString("Got change DRIVEADD %1.").arg(n1);
               emit FileSystemWatcher::obj().driveAdded(n1);
            break;}
            case SHCNE_DRIVEREMOVED: {
               qDebug() << QString("Got change DRIVEREMOVED %1.").arg(n1);
               emit FileSystemWatcher::obj().driveRemoved(n1);
            break;}

            case SHCNE_MEDIAINSERTED: {
               qDebug() << QString("Got change MEDIAINSERTED %1.").arg(n1);
               emit FileSystemWatcher::obj().mediaInserted(n1);
            break;}
            case SHCNE_MEDIAREMOVED: {
               qDebug() << QString("Got change MEDIAREMOVED %1.").arg(n1);
               emit FileSystemWatcher::obj().mediaRemoved(n1);
            break;}

            case SHCNE_MKDIR: {
               qDebug() << QString("Got change MKDIR %1.").arg(n1);
               emit FileSystemWatcher::obj().folderCreated(n1);
            break;}
            case SHCNE_RENAMEFOLDER: {
               qDebug() << QString("Got change RENAMEFOLDER %1 to %2.").arg(n1).arg(n2);
               emit FileSystemWatcher::obj().folderRenamed(n1, n2);
            break;}
            case SHCNE_RMDIR: {
               qDebug() << QString("Got change RMDIR %1.").arg(n1);
               emit FileSystemWatcher::obj().folderDeleted(n1);
            break;}
            case SHCNE_UPDATEDIR: {
               qDebug() << QString("Got change UPDATEDIR %1.").arg(n1);
               emit FileSystemWatcher::obj().folderChanged(n1);
            break;}

            default: qDebug() << "Got unrecognized change.";
        }

        SHChangeNotification_Unlock(lock);
//        result = 0;
        return true;
    }
}
