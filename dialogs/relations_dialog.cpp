#include "relations_dialog.h"
#include "ui_relations_dialog.h"

#include "modules/models/service/relations_delegate.h"
#include "modules/core/web/web_apis.h"

using namespace Core;
using namespace Web;

void RelationsDialog::prepareLinkablesList(const QList<Web::Linkable> & linkables, QListWidget * list) {
    for(QList<Web::Linkable>::ConstIterator linkable = linkables.cbegin(); linkable != linkables.cend(); linkable++) {
        QListWidgetItem * item = new QListWidgetItem((*linkable).humanName(), list);
        item -> setData(Qt::UserRole + 2, (*linkable).imageUrl());
        item -> setData(Qt::UserRole + 1, (*linkable).permaTitle());
        item -> setData(Qt::UserRole, (*linkable).uid());
        list -> addItem(item);
    }

    list -> setItemDelegate(new RelationsDelegate(list));
    list -> setEditTriggers(QListView::NoEditTriggers);
}

RelationsDialog::RelationsDialog(ISource * currApi, QWidget * parent)
    : BaseDialog(parent), ui(new Ui::RelationsDialog) {

    api = dynamic_cast<Web::Sociable *>(currApi);

    ui -> setupUi(this);

    prepareLinkablesList(api -> friendsList(), ui -> friendsList);
    ui -> friendsList -> sortItems();
    prepareLinkablesList(api -> groupsList(), ui -> groupsList);
    ui -> groupsList -> sortItems();

    bool by_friend_id = api -> hasSearchByFriendId();

    ui -> friendId -> setVisible(by_friend_id);
    ui -> friendById -> setVisible(by_friend_id);

    bool by_friend_name = api -> hasSearchByFriendName();

    ui -> friendName -> setVisible(by_friend_name);
    ui -> friendByName -> setVisible(by_friend_name);

    bool by_group_id = api -> hasSearchByFriendId();

    ui -> groupId -> setVisible(by_group_id);
    ui -> groupById -> setVisible(by_group_id);

    bool by_group_name = api -> hasSearchByFriendName();

    ui -> groupName -> setVisible(by_group_name);
    ui -> groupByName -> setVisible(by_group_name);
}

RelationsDialog::~RelationsDialog() {
    delete ui;
}

void RelationsDialog::on_cancelButton_clicked() { reject(); }

void RelationsDialog::on_friendsList_itemActivated(QListWidgetItem * item) {
    uid = item -> data(Qt::UserRole).toString();
    name = item -> text();

    accept();
}

void RelationsDialog::on_groupsList_itemActivated(QListWidgetItem * item) {
    uid = QStringLiteral("-") % item -> data(Qt::UserRole).toString();
    name = item -> text();

    accept();
}

void RelationsDialog::on_friendById_clicked() {
    ui -> friendsList -> clear();
    QList<Linkable> friends = api -> findFriendsById(ui -> friendId -> text());
    prepareLinkablesList(friends, ui -> friendsList);
}

void RelationsDialog::on_friendByName_clicked() {
    ui -> friendsList -> clear();
    QList<Linkable> friends = api -> findFriendsByName(ui -> friendName -> text());
    prepareLinkablesList(friends, ui -> friendsList);
}

void RelationsDialog::on_groupById_clicked() {
    ui -> groupsList -> clear();
    QList<Linkable> groups = api -> findGroupsById(ui -> groupId -> text());
    prepareLinkablesList(groups, ui -> groupsList);

//    if (uid.isEmpty()) return;

//    name = QStringLiteral("Group ") % uid;
//    uid = QStringLiteral("-") % uid;
//    accept();
}

void RelationsDialog::on_groupByName_clicked() {
    ui -> groupsList -> clear();
    QList<Linkable> groups = api -> findGroupsByName(ui -> groupName -> text());
    prepareLinkablesList(groups, ui -> groupsList);
}
