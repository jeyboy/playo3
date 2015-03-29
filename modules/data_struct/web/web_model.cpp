#include "web_model.h"

using namespace Playo3;
///////////////////////////////////////////////////////////

WebModel::WebModel(QString uid, QJsonObject * hash, QObject *parent) : TreeModel(hash, parent), DeletedList() {
    tabUid = uid;

    if (hash)
        deletedFromJson(hash -> value("deleted").toArray());
}

WebModel::~WebModel() {
}

QString WebModel::getTabUid() const {
    return tabUid;
}

void WebModel::deleteRemoved(QHash<IItem *, QString> & store) {
    qDebug() << "STORE LENGTH: " << store.count();
    foreach(ModelItem * item, store.keys()) {
        if (Player::instance() -> playedItem() == item)
            Player::instance() -> setPlaylist(0);

        qDebug() << "DELETE : " << item -> fullPath();
        QModelIndex ind = index(item);
        removeRow(ind.row(), ind.parent());
    }
}

void WebModel::errorReceived(int, QString msg) {
    emit showMessage("!!!!!!!!!!! Some shit happened :( " + msg);
    emit hideSpinner();
}
