//#include "relationdialog.h"
//#include "ui_relationdialog.h"
//#include <QDebug>

//RelationDialog::RelationDialog(WebApi * currApi, QWidget *parent) : QDialog(parent), ui(new Ui::RelationDialog) {
//    ui->setupUi(this);

//    api = currApi;

//    friendModel = new QStringListModel(api -> friendsList().values());
//    ui -> friendsList -> setModel(friendModel);
//    friendModel -> sort(0);

//    groupModel = new QStringListModel(api -> groupsList().values());
//    ui -> groupsList -> setModel(groupModel);
//    groupModel -> sort(0);

//    ui -> friendsList -> setEditTriggers(QListView::NoEditTriggers);
//    ui -> groupsList -> setEditTriggers(QListView::NoEditTriggers);
//}

//RelationDialog::~RelationDialog() {
//    delete ui;
//    delete friendModel;
//    delete groupModel;
//}

//int RelationDialog::getId() const {
//    return uid;
//}

//QString RelationDialog::getName() {
//    return name;
//}

//void RelationDialog::on_cancelButton_clicked() {
//    reject();
//}

//void RelationDialog::on_friendsList_activated(const QModelIndex &index) {
//    name = index.data().toString();
//    uid = api -> friendsList().key(name).toInt();
//    accept();
//}

//void RelationDialog::on_groupsList_activated(const QModelIndex &index) {
//    name = index.data().toString();
//    uid = api -> groupsList().key(name).toInt() * -1;
//    accept();
//}
