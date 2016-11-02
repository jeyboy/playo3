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
        item -> setToolTip((*linkable).humanName() % LSTR(" *** ") % (*linkable).permaTitle());
        list -> addItem(item);
    }

    list -> setItemDelegate(new RelationsDelegate(list));
    list -> setEditTriggers(QListView::NoEditTriggers);
}

RelationsDialog::RelationsDialog(ISource * currApi, QWidget * parent)
    : BaseDialog(parent), ui(new Ui::RelationsDialog) {

    api = dynamic_cast<Web::Sociable *>(currApi);

    ui -> setupUi(this);
    ui -> errorStr -> setVisible(false);

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

void RelationsDialog::on_friendsList_itemActivated(QListWidgetItem * item) {
    emit relationTabCreationRequired(item -> text(), item -> data(Qt::UserRole).toString());
}

void RelationsDialog::on_groupsList_itemActivated(QListWidgetItem * item) {
    emit relationTabCreationRequired(item -> text(), QStringLiteral("-") % item -> data(Qt::UserRole).toString());
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

void RelationsDialog::on_closeButton_clicked() { close(); }

void RelationsDialog::on_friendPredicate_textChanged(const QString & text) {
    bool is_empty = text.isEmpty();

    for(int index = 0; index < ui -> friendsList -> count(); index++)
        ui -> friendsList -> setRowHidden(index, !is_empty && !ui -> friendsList -> item(index) -> text().contains(text, Qt::CaseInsensitive));
}

void RelationsDialog::on_groupPredicate_textChanged(const QString & text) {
    bool is_empty = text.isEmpty();

    for(int index = 0; index < ui -> groupsList -> count(); index++)
        ui -> groupsList -> setRowHidden(index, !is_empty && !ui -> groupsList -> item(index) -> text().contains(text, Qt::CaseInsensitive));
}
