#include "relations_dialog.h"
#include "ui_relations_dialog.h"

RelationsDialog::RelationsDialog(Web::Sociable * currApi, QWidget * parent)
    : QDialog(parent), ui(new Ui::RelationsDialog), api(currApi) {

    ui -> setupUi(this);

    friendModel = new QStringListModel(api -> friendsList().values());
    friendModel -> sort(0);
    ui -> friendsList -> setModel(friendModel);
    ui -> friendsList -> setEditTriggers(QListView::NoEditTriggers);

    groupModel = new QStringListModel(api -> groupsList().values());
    groupModel -> sort(0);
    ui -> groupsList -> setModel(groupModel);
    ui -> groupsList -> setEditTriggers(QListView::NoEditTriggers);
}

RelationsDialog::~RelationsDialog() {
    delete ui;
    delete friendModel;
    delete groupModel;
}

void RelationsDialog::on_cancelButton_clicked() {
    reject();
}

void RelationsDialog::on_friendsList_activated(const QModelIndex & index) {
    name = index.data().toString();
    uid = api -> friendsList().key(name);
    accept();
}

void RelationsDialog::on_groupsList_activated(const QModelIndex & index) {
    name = index.data().toString();
    uid = QStringLiteral("-") % api -> groupsList().key(name);
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
