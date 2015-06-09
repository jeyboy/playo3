#ifndef LOADER
#define LOADER

#include <qdir.h>
#include <qapplication.h>
#include <QPluginLoader>

#include "web_dialog_interface.h"

static bool loadWebDialogPlugin(WebDialogInterface *& wdi) {
    QDir pluginsDir(QApplication::instance() -> applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject * plugin = pluginLoader.instance();
        if (plugin) {
            wdi = qobject_cast<WebDialogInterface *>(plugin);
            if (wdi)
                return true;
        }
    }

    return false;
}

#endif // LOADER
