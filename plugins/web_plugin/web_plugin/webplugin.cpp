#include "webplugin.h"

QDialog * WebPlugin::createDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl url, QString title) {
    return (dialog = new WebDialog(parent, manager, url, title));
}

void WebPlugin::closeDialog() {
    delete dialog;
    dialog = 0;
}
