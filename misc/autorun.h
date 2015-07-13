#ifndef AUTORUN
#define AUTORUN

#include <qsettings.h>
#include <qapplication.h>

//NOT USED

#define AUTORUN_NAME QStringLiteral("Playo3")

class Autorun {
public:
    static void registerApp() {
        #ifdef Q_OS_WIN
    //        HKMU/Software/Microsoft/Windows/CurrentVersion/Run
            QSettings settings(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);

            QString value = QCoreApplication::applicationFilePath();
            value = value.replace(QStringLiteral("/"), QStringLiteral("\\"));

            settings.setValue(AUTORUN_NAME, QStringLiteral("\"") + value + QStringLiteral("\""));
        #else
//        mkdir -p ~/.config/autostart/
//        .. and add a <Desktop entry> ( AppName.desktop ),
//        or a link to the executable in ~.config/autostart/

        //For Linux you have to create an entry file for your application in '$HOME/.config/autostart'.
        #endif
    }

    static void unregisterApp() {
        #ifdef Q_OS_WIN
            QSettings settings(QStringLiteral("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
            settings.remove(AUTORUN_NAME);
        #else

        #endif
    }
};

#endif // AUTORUN
