#ifndef AUTORUN
#define AUTORUN

#include <qsettings.h>
#include <qapplication.h>

//NOT USED

class Autorun {
public:
    void registerApp() {

        #ifdef Q_OS_WIN
    //        HKMU/Software/Microsoft/Windows/CurrentVersion/Run
            QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

            QString value = QCoreApplication::applicationFilePath();
            value = value.replace("/", "\\");

            settings.setValue("Playo3", "\"" + value + "\"");
        #else
//        mkdir -p ~/.config/autostart/
//        .. and add a <Desktop entry> ( AppName.desktop ),
//        or a link to the executable in ~.config/autostart/

        //For Linux you have to create an entry file for your application in '$HOME/.config/autostart'.
        #end
    }

    void unregisterApp() {
        #ifdef Q_OS_WIN
            QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            settings.remove("Playo3");
        #else

        #end
    }
};

#endif // AUTORUN
