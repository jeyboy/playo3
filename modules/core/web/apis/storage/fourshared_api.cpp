#include "fourshared_api.h"

#include <qmenu.h>

using namespace Core::Web::Fourshared;

void Api::fromJson(const QJsonObject & hash) {
    QJsonObject obj = hash.value(name()).toObject();
    ISource::fromJson(obj);
    TeuAuth::fromJson(obj);

    Manager::addCookie(val_lang_cookie);
}
void Api::toJson(QJsonObject & hash) {
    QJsonObject root;

    ISource::toJson(root);
    TeuAuth::toJson(root);

    hash.insert(name(), root);
}

//QToolButton * Api::initButton(QWidget * parent) {
//    if (button == 0) {
//        if (!parent) {
//            qDebug() << "PIPEC" << name();
//            return 0;
//        }
//        button = new QToolButton(parent);
//    } else {
//        button -> setMenu(0);
//        disconnect(button, SIGNAL(clicked()), this, SLOT(openTab()));
//    }

//    if (isConnected()) {
//        button -> setIcon(QIcon(QStringLiteral(":/add_4shared_on")));
//        button -> setPopupMode(QToolButton::InstantPopup);
//        button -> setToolTip(QStringLiteral("4shared(4shared.com)"));

//        QMenu * menu = new QMenu(button);
//        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
//        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));
//        button -> setMenu(menu);
//    } else {
//        button -> setIcon(QIcon(QStringLiteral(":/add_4shared")));
//        button -> setToolTip(QStringLiteral("Connect to 4shared(4shared.com)"));
//        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
//    }

//    return button;
//}
