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

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(onFocusChanged(QWidget*,QWidget*)));

    api = dynamic_cast<Web::Sociable *>(currApi);

    ui -> setupUi(this);
    ui -> errorStr -> setVisible(false);
    default_btn = ui -> cancelButton;

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

RelationsDialog::~RelationsDialog() { delete ui; }

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
    ui -> errorStr -> setVisible(false);
    ui -> friendsList -> clear();
    QList<Linkable> friends = api -> findFriendsById(ui -> friendId -> text());
    if (friends.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(QStringLiteral("Not found anything"));
    }
    else prepareLinkablesList(friends, ui -> friendsList);
}

void RelationsDialog::on_friendByName_clicked() {
    ui -> errorStr -> setVisible(false);
    ui -> friendsList -> clear();
    QList<Linkable> friends = api -> findFriendsByName(ui -> friendName -> text());

    if (friends.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(QStringLiteral("Not found anything"));
    }
    else prepareLinkablesList(friends, ui -> friendsList);
}

void RelationsDialog::on_groupById_clicked() {
    ui -> errorStr -> setVisible(false);
    ui -> groupsList -> clear();
    QList<Linkable> groups = api -> findGroupsById(ui -> groupId -> text());

    if (groups.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(QStringLiteral("Not found anything"));
    }
    else prepareLinkablesList(groups, ui -> groupsList);
}

void RelationsDialog::on_groupByName_clicked() {
    ui -> errorStr -> setVisible(false);
    ui -> groupsList -> clear();
    QList<Linkable> groups = api -> findGroupsByName(ui -> groupName -> text());
    if (groups.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(QStringLiteral("Not found anything"));
    }
    else prepareLinkablesList(groups, ui -> groupsList);
}

void RelationsDialog::onFocusChanged(QWidget * /*old*/, QWidget * now) {
    default_btn -> setDefault(false);

    bool fid = now == ui -> friendId;
    if (fid) default_btn = ui -> friendById;

    bool fname = now == ui -> friendName;
    if (fname) default_btn = ui -> friendByName;

    bool gid = now == ui -> groupId;
    if (gid) default_btn = ui -> groupById;

    bool gname = now == ui -> groupName;
    if (gname) default_btn = ui -> groupByName;

    if (!(fid | fname | gid | gname))
        default_btn = ui -> cancelButton;

    default_btn -> setDefault(true);
}
