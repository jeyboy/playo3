#include "webplugin.h"

QDialog * WebPlugin::createDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl url, QString title) {
    return (dialog = new WebDialog(parent, manager, url, title));
}

void WebPlugin::registerActions(QObject * api) {
    connect(dialog, SIGNAL(urlChanged(const QUrl &)), api, SLOT(proceedAuthResponse(QUrl)));
    connect(api, SIGNAL(responseReady(QString)), dialog, SLOT(setApiResponse(QString)));
}

void WebPlugin::closeDialog() {
    delete dialog;
    dialog = 0;
}
