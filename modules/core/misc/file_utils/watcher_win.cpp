//#include <qt_windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <qlibrary.h>
#include <qdebug.h>

#include "watcher.h"

//typedef struct {
//    LPCITEMIDLIST pidl;
//    BOOL fRecursive;
//} SHChangeNotifyEntry;

//#define SHELL_ENTRY(returnType, functionName, parameters) \
//typedef returnType __stdcall (*functionName##_type) parameters; \
//const functionName##_type functionName \
//= (functionName##_type) QLibrary::resolve("shell32", #functionName)

//SHELL_ENTRY(ULONG, SHChangeNotifyRegister,
//           (HWND hwnd, int fSources, LONG fEvents, UINT wMsg, int cEntries, SHChangeNotifyEntry * pshcne));
//SHELL_ENTRY(BOOL, SHChangeNotifyDeregister, (unsigned long ulID));
//SHELL_ENTRY(HANDLE, SHChangeNotification_Lock,
//           (HANDLE hChangeNotification, DWORD dwProcessId, LPITEMIDLIST ** pppidl, LONG * plEvent));
//SHELL_ENTRY(BOOL, SHChangeNotification_Unlock, (HANDLE hLock));

namespace {
    class WinWatcher : public QWidget, public WatcherCell {
        static const unsigned int msgShellChange = WM_USER + 1;
        unsigned long id;
        static QString getPidlPath(ITEMIDLIST * pidl);
        ITEMIDLIST * dir;
        public:
            WinWatcher(const QString & path, bool recursive);
            ~WinWatcher();

            bool isValid();
        protected:
//            bool winEvent(MSG * message, long * result);
            bool nativeEvent(const QByteArray & eventType, void * message, long * result);
    };

    WinWatcher::WinWatcher(const QString & path, bool recursive) : QWidget(), id(0) {
        dir = ILCreateFromPathW(path.toStdWString().c_str()); // this requires on test
//        qDebug() << "~~~~~~~~~~~~~" << WinWatcher::getPidlPath(dir);

        SHChangeNotifyEntry ne = {dir, recursive};

        id = SHChangeNotifyRegister((HWND)winId(), SHCNRF_InterruptLevel | SHCNRF_ShellLevel | SHCNRF_RecursiveInterrupt | SHCNRF_NewDelivery,
                                    SHCNE_ATTRIBUTES       |
                                    SHCNE_CREATE           |
                                    SHCNE_DELETE           |
                                    SHCNE_DRIVEADD         |
                                    SHCNE_DRIVEADDGUI      |
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
    }

    bool WinWatcher::isValid() { return id > 0; }

    WinWatcher::~WinWatcher() {
        if (id) {
            SHChangeNotifyDeregister(id);
            ILFree(dir);
        }
    }

//    QString WinWatcher::getPidlPath(ITEMIDLIST * pidl) {
//        if (!pidl) return "";
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
//    }

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

    bool WinWatcher::nativeEvent(const QByteArray & eventType, void * message, long * result) {
        if (eventType != "windows_generic_MSG") return false;
        MSG * msg = (MSG *)message;
        if (msg -> message != msgShellChange) return false;

        long event;
        ITEMIDLIST ** items;
        HANDLE lock = SHChangeNotification_Lock((HANDLE) msg -> wParam, msg -> lParam, &items, &event);
        QString n1 = QDir::fromNativeSeparators(getPidlPath(items[0]));
        QString n2 = QDir::fromNativeSeparators(getPidlPath(items[1]));

        switch (event) {
            case SHCNE_ATTRIBUTES: {
               qDebug() << QString("Got change ATTRIBUTES for %1.").arg(n1);
               emit Watcher::obj().attributeChanged(n1);
            break;}

            case SHCNE_CREATE: {
               qDebug() << QString("Got change CREATE for %1.").arg(n1);
               emit Watcher::obj().fileCreated(n1);
            break;}
            case SHCNE_DELETE: {
               qDebug() << QString("Got change DELETE %1.").arg(n1);
               emit Watcher::obj().fileDeleted(n1);
            break;}
            case SHCNE_RENAMEITEM: {
               qDebug() << QString("Got change RENAMEITEM %1 to %2.").arg(n1).arg(n2);
               emit Watcher::obj().fileRenamed(n1, n2);
            break;}
            case SHCNE_UPDATEITEM: {
               qDebug() << QString("Got change UPDATEITEM %1.").arg(n1);
               emit Watcher::obj().fileChanged(n1);
            break;}

            case SHCNE_DRIVEADD: {
               qDebug() << QString("Got change DRIVEADD %1.").arg(n1);
               emit Watcher::obj().driveAdded(n1);
            break;}
            case SHCNE_DRIVEADDGUI: {
               qDebug() << QString("Got change DRIVEADDGUI %1.").arg(n1);
               emit Watcher::obj().driveGuiAdded(n1);
            break;}
            case SHCNE_DRIVEREMOVED: {
               qDebug() << QString("Got change DRIVEREMOVED %1.").arg(n1);
               emit Watcher::obj().driveRemoved(n1);
            break;}

            case SHCNE_MEDIAINSERTED: {
               qDebug() << QString("Got change MEDIAINSERTED %1.").arg(n1);
               emit Watcher::obj().mediaInserted(n1);
            break;}
            case SHCNE_MEDIAREMOVED: {
               qDebug() << QString("Got change MEDIAREMOVED %1.").arg(n1);
               emit Watcher::obj().mediaRemoved(n1);
            break;}

            case SHCNE_MKDIR: {
               qDebug() << QString("Got change MKDIR %1.").arg(n1);
               emit Watcher::obj().folderCreated(n1);
            break;}
            case SHCNE_RENAMEFOLDER: {
               qDebug() << QString("Got change RENAMEFOLDER %1 to %2.").arg(n1).arg(n2);
               emit Watcher::obj().folderRenamed(n1, n2);
            break;}
            case SHCNE_RMDIR: {
               qDebug() << QString("Got change RMDIR %1.").arg(n1);
               emit Watcher::obj().folderDeleted(n1);
            break;}
            case SHCNE_UPDATEDIR: {
               qDebug() << QString("Got change UPDATEDIR %1.").arg(n1);
               emit Watcher::obj().folderChanged(n1);
            break;}

            default: qDebug() << "Got unrecognized change.";
        }

        SHChangeNotification_Unlock(lock);
        result = 0;
        return true;
    }
}
