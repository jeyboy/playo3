#ifndef LOADER
#define LOADER

#include <qdir.h>
#include <qapplication.h>
#include <qpluginloader.h>

#include "misc/logger.h"
#include "web_dialog_interface.h"

static bool loadWebDialogPlugin(WebDialogInterface *& wdi) {
    QDir pluginsDir(QApplication::instance() -> applicationDirPath());
    QStringList filters;

    filters
        #ifdef Q_OS_WIN
            << "web_plugin.dll";
//        #else ifdef Q_OS_DARWIN
        #else
            << "libweb_plugin.so" << "libweb_plugin.so.1" << "libweb_plugin.so.1.0" << "libweb_plugin.so.1.0.0";
        #endif

    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject * plugin = pluginLoader.instance();
        if (plugin) {
            wdi = qobject_cast<WebDialogInterface *>(plugin);
            if (wdi)
                return true;
        }
        else Logger::instance() -> writeToStream("Plugin loader", pluginLoader.errorString());
    }

    return false;
}

#endif // LOADER
