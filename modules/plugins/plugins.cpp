#include "plugins.h"

bool Plugins::loadWebDialog(WebDialogInterface *& wdi) {
    QDir pluginsDir(QApplication::instance() -> applicationDirPath());
    QStringList filters;

    filters
        #ifdef Q_OS_WIN
            << QStringLiteral("web_plugin.dll");
//        #else ifdef Q_OS_DARWIN
        #else
            << QStringLiteral("libweb_plugin.so") << QStringLiteral("libweb_plugin.so.1") << QStringLiteral("libweb_plugin.so.1.0") << QStringLiteral("libweb_plugin.so.1.0.0");
        #endif

    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject * plugin = pluginLoader.instance();
        if (plugin) {
            wdi = qobject_cast<WebDialogInterface *>(plugin);
            if (wdi)
                return true;
        }
        else Logger::instance() -> writeToStream(QStringLiteral("Plugin loader"), pluginLoader.errorString());
    }

    return false;
}
