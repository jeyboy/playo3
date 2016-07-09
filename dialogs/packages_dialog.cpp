#include "packages_dialog.h"
#include "ui_packages_dialog.h"

#include "modules/models/service/relations_delegate.h"
#include "modules/core/web/web_apis.h"

using namespace Core;

//void SetsDialog::prepareLinkablesList(const QList<Web::Linkable> & linkables, QListWidget * list) {
//    for(QList<Web::Linkable>::ConstIterator linkable = linkables.cbegin(); linkable != linkables.cend(); linkable++) {
//        QListWidgetItem * item = new QListWidgetItem((*linkable).humanName(), list);
//        item -> setData(Qt::UserRole + 2, (*linkable).imageUrl());
//        item -> setData(Qt::UserRole + 1, (*linkable).permaTitle());
//        item -> setData(Qt::UserRole, (*linkable).uid());
//        list -> addItem(item);
//    }

//    list -> setItemDelegate(new RelationsDelegate(list));
//    list -> setEditTriggers(QListView::NoEditTriggers);
//}

PackagesDialog::PackagesDialog(ISource * currApi, QWidget * parent)
    : BaseDialog(parent), ui(new Ui::PackagesDialog) {

    ui -> setupUi(this);

    QMap<QString, QString> customs = currApi -> customsList();

    for(QMap<QString, QString>::ConstIterator custom = customs.cbegin(); custom != customs.cend(); custom++) {
        QListWidgetItem * item = new QListWidgetItem(custom.key(), ui -> packagesList);
//        item -> setData(Qt::UserRole + 2, (*linkable).imageUrl());
        item -> setData(Qt::UserRole + 1, currApi -> name());
        item -> setData(Qt::UserRole, custom.value());
        ui -> packagesList -> addItem(item);
    }

    ui -> packagesList -> setItemDelegate(new RelationsDelegate(ui -> packagesList));
    ui -> packagesList -> setEditTriggers(QListView::NoEditTriggers);
//    ui -> groupsList -> sortItems();
}

PackagesDialog::~PackagesDialog() { delete ui; }

void PackagesDialog::on_cancelButton_clicked() { reject(); }

void PackagesDialog::on_packagesList_itemActivated(QListWidgetItem * item) {
    params = item -> data(Qt::UserRole).toString();
    name = item -> text();

    accept();
}
