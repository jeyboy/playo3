#include "relations_dialog.h"
#include "ui_relations_dialog.h"

#include "modules/models/service/relations_delegate.h"
#include "modules/core/web/web_apis.h"
#include "modules/core/media/image_bank.h"

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

    if (currApi -> hasUsers()) {
        prepareLinkablesList(api -> friendsList(), ui -> friendsList);
        ui -> friendsList -> sortItems();

        bool by_friend_id = api -> hasSearchByFriendId();
        ui -> friendById -> setVisible(by_friend_id);

        bool by_friend_name = api -> hasSearchByFriendName();
        ui -> friendByName -> setVisible(by_friend_name);
    }
    else ui -> tabWidget -> setTabEnabled(0, false);

    if (currApi -> hasGroups()) {
        prepareLinkablesList(api -> groupsList(), ui -> groupsList);
        ui -> groupsList -> sortItems();

        bool by_group_id = api -> hasSearchByGroupId();
        ui -> groupById -> setVisible(by_group_id);

        bool by_group_name = api -> hasSearchByGroupName();
        ui -> groupByName -> setVisible(by_group_name);
    }
    else ui -> tabWidget -> setTabEnabled(1, false);
}

void RelationsDialog::done(int status) {
    ui -> friendsList -> clear();
    ui -> groupsList -> clear();

    ImageBank::obj().cancelPackets(ui -> friendsList -> model());
    ImageBank::obj().cancelPackets(ui -> groupsList -> model());

    BaseDialog::done(status);
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
    QList<Linkable> friends = api -> findFriendsById(ui -> friendPredicate -> text());
    if (friends.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(EMPTY_RESULT_MESSAGE);
    }
    else prepareLinkablesList(friends, ui -> friendsList);
}

void RelationsDialog::on_friendByName_clicked() {
    ui -> errorStr -> setVisible(false);
    ui -> friendsList -> clear();
    QList<Linkable> friends = api -> findFriendsByName(ui -> friendPredicate -> text());

    if (friends.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(EMPTY_RESULT_MESSAGE);
    }
    else prepareLinkablesList(friends, ui -> friendsList);
}

void RelationsDialog::on_groupById_clicked() {
    ui -> errorStr -> setVisible(false);
    ui -> groupsList -> clear();
    QList<Linkable> groups = api -> findGroupsById(ui -> groupPredicate -> text());

    if (groups.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(EMPTY_RESULT_MESSAGE);
    }
    else prepareLinkablesList(groups, ui -> groupsList);
}

void RelationsDialog::on_groupByName_clicked() {
    ui -> errorStr -> setVisible(false);
    ui -> groupsList -> clear();
    QList<Linkable> groups = api -> findGroupsByName(ui -> groupPredicate -> text());
    if (groups.isEmpty()) {
        ui -> errorStr -> setVisible(true);
        ui -> errorStr -> setText(EMPTY_RESULT_MESSAGE);
    }
    else prepareLinkablesList(groups, ui -> groupsList);
}

void RelationsDialog::onFocusChanged(QWidget * /*old*/, QWidget * now) {
    default_btn -> setDefault(false);

    bool fid = now == ui -> friendPredicate;
    if (fid) default_btn = ui -> friendInList;

    bool gid = now == ui -> groupPredicate;
    if (gid) default_btn = ui -> groupInList;


    if (!(fid | gid))
        default_btn = ui -> cancelButton;

    default_btn -> setDefault(true);
}

void RelationsDialog::on_friendInList_clicked() {
    QString predicate = ui -> friendPredicate -> text();
    bool is_empty = predicate.isEmpty();

    for(int index = 0; index < ui -> friendsList -> count(); index++)
        ui -> friendsList -> setRowHidden(index, !is_empty && !ui -> friendsList -> item(index) -> text().contains(predicate, Qt::CaseInsensitive));
}

void RelationsDialog::on_groupInList_clicked() {
    QString predicate = ui -> groupPredicate -> text();
    bool is_empty = predicate.isEmpty();

    for(int index = 0; index < ui -> groupsList -> count(); index++)
        ui -> groupsList -> setRowHidden(index, !is_empty && !ui -> groupsList -> item(index) -> text().contains(predicate, Qt::CaseInsensitive));
}
