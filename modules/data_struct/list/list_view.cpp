#include "list_view.h"
#include <QDebug>

ListView::ListView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : View(dynamic_cast<Model *>(new ListModel(hash)), parent, settingsSet) {
}

ListView::~ListView() {

}
