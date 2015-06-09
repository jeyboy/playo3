#ifndef WEBPLUGIN_H
#define WEBPLUGIN_H

#include <qobject.h>
#include "web_plugin_global.h"
#include "web_dialog_interface.h"
#include "web_dialog.h"

class WEB_PLUGINSHARED_EXPORT WebPlugin : public QObject, WebDialogInterface {
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID "org.qt-project.lalka.WebDialogInterface" FILE "webplugin.json")
    Q_PLUGIN_METADATA(IID "org.qt-project.lalka.WebDialogInterface")
    Q_INTERFACES(WebDialogInterface)
public:
    WebPlugin() : dialog(0) {}
    ~WebPlugin() { delete dialog; }
    QDialog * createDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl url, QString title);
    void closeDialog();
protected:
    WebDialog * dialog;
};

#endif // WEBPLUGIN_H
