#ifndef LOADER
#define LOADER

#include <qdir.h>
#include <qapplication.h>
#include <QPluginLoader>
#include <qdebug.h>

#include "web_dialog_interface.h"

static bool loadWebDialogPlugin(WebDialogInterface *& wdi) {
    QDir pluginsDir(QApplication::instance() -> applicationDirPath());
    QStringList filters;
    filters << "web_plugin.dll";

    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        qDebug() << pluginLoader.errorString();
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
