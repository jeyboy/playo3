#ifndef AUTORUN
#define AUTORUN

#include <qsettings.h>
#include <qapplication.h>
#include <qstandardpaths.h>
#include <qfile.h>

#define AUTORUN_NAME QLatin1String("Playo3")

class Autorun {
    inline static QString startup_path() { return QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) % QLatin1String("/Startup/"); }
public:
    static void registerApp() {
        #ifdef Q_OS_WIN
            QFile::link(QCoreApplication::applicationFilePath(), startup_path() % AUTORUN_NAME % ".lnk");
        #else
//        mkdir -p ~/.config/autostart/
//        .. and add a <Desktop entry> ( AppName.desktop ),
//        or a link to the executable in ~.config/autostart/

        //For Linux you have to create an entry file for your application in '$HOME/.config/autostart'.
        #endif
    }

    static void unregisterApp() {
        #ifdef Q_OS_WIN
            QSettings settings(QLatin1String("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
            settings.remove(AUTORUN_NAME);

            QFile::remove(startup_path() % AUTORUN_NAME % ".lnk");
        #else

        #endif
    }

//    static void registerApp() {
//        #ifdef Q_OS_WIN
//    //        HKMU/Software/Microsoft/Windows/CurrentVersion/Run
//            QSettings settings(QLatin1String("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);

//            QString value = QCoreApplication::applicationFilePath();
//            value = value.replace(QLatin1String("/"), QLatin1String("\\"));

//            settings.setValue(AUTORUN_NAME, QLatin1String("\"") + value + QLatin1String("\""));
//        #else
////        mkdir -p ~/.config/autostart/
////        .. and add a <Desktop entry> ( AppName.desktop ),
////        or a link to the executable in ~.config/autostart/

//        //For Linux you have to create an entry file for your application in '$HOME/.config/autostart'.
//        #endif
//    }

//    static void unregisterApp() {
//        #ifdef Q_OS_WIN
//            QSettings settings(QLatin1String("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
//            settings.remove(AUTORUN_NAME);
//        #else

//        #endif
//    }
};

#endif // AUTORUN
