#include "relationdialog.h"
#include "ui_relationdialog.h"

RelationDialog::RelationDialog(WebApi * currApi, QWidget * parent)
    : QDialog(parent), ui(new Ui::RelationDialog), api(currApi) {

    ui -> setupUi(this);

    friendModel = new QStringListModel(/*api -> friendsList().values()*/);
    friendModel -> sort(0);
    ui -> friendsList -> setModel(friendModel);
    ui -> friendsList -> setEditTriggers(QListView::NoEditTriggers);

    groupModel = new QStringListModel(api -> groupsList().values());
    groupModel -> sort(0);
    ui -> groupsList -> setModel(groupModel);
    ui -> groupsList -> setEditTriggers(QListView::NoEditTriggers);
}

RelationDialog::~RelationDialog() {
    delete ui;
    delete friendModel;
    delete groupModel;
}

void RelationDialog::on_cancelButton_clicked() {
    reject();
}

void RelationDialog::on_friendsList_activated(const QModelIndex & index) {
    name = index.data().toString();
    uid = api -> friendsList().key(name).toInt();
    accept();
}

void RelationDialog::on_groupsList_activated(const QModelIndex & index) {
    name = index.data().toString();
    uid = -api -> groupsList().key(name).toInt();
    accept();
}
