#include "relations_dialog.h"
#include "ui_relations_dialog.h"

#include "modules/models/service/relations_delegate.h"

using namespace Core;
using namespace Web;

void RelationsDialog::prepareLinkablesList(QHash<QString, Web::Linkable> linkables, QListWidget * list) {
    for(QHash<QString, Linkable>::Iterator linkable = linkables.begin(); linkable != linkables.end(); linkable++) {
        // icons stubed at this time
        QListWidgetItem * item = new QListWidgetItem(QIcon(":main"), linkable.value().humanName(), list);
        item -> setData(Qt::UserRole + 2, linkable.value().imageUrl());
        item -> setData(Qt::UserRole + 1, linkable.value().permaTitle());
        item -> setData(Qt::UserRole, linkable.value().uid());
        list -> addItem(item);
    }

    list -> setItemDelegate(new RelationsDelegate(list));
    list -> setEditTriggers(QListView::NoEditTriggers);
    list -> sortItems();
}

RelationsDialog::RelationsDialog(Sociable * currApi, QWidget * parent)
    : QDialog(parent), ui(new Ui::RelationsDialog), api(currApi) {

    ui -> setupUi(this);

    prepareLinkablesList(api -> friendsList(), ui -> friendsList);
    prepareLinkablesList(api -> groupsList(), ui -> groupsList);
}

RelationsDialog::~RelationsDialog() {
    delete ui;
}

void RelationsDialog::on_cancelButton_clicked() { reject(); }

void RelationsDialog::on_friendsList_itemActivated(QListWidgetItem * item) {
    Linkable linkable = api -> friendsList().value((uid = item -> data(Qt::UserRole).toString()));
    name = linkable.humanName();
    accept();
}

void RelationsDialog::on_groupsList_itemActivated(QListWidgetItem * item) {
    Linkable linkable = api -> friendsList().value((uid = item -> data(Qt::UserRole).toString()));
    name = linkable.humanName();
    uid = QStringLiteral("-") % uid;
    accept();
}

void RelationsDialog::on_friendManually_clicked() {
    uid = ui -> friendId -> text();

    if (uid.isEmpty()) return;

    name = QStringLiteral("User ") % uid;
    accept();
}

void RelationsDialog::on_groupManually_clicked() {
    uid = ui -> groupId -> text();

    if (uid.isEmpty()) return;

    name = QStringLiteral("Group ") % uid;
    uid = QStringLiteral("-") % uid;
    accept();
}
