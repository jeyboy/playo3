#include "packages_dialog.h"
#include "ui_packages_dialog.h"

#include "modules/models/service/relations_delegate.h"
#include "modules/core/web/web_apis.h"

using namespace Core;

bool PackagesDialog::takeGenre(QString & genre) {
    UserActionDialog dialog(parentWidget());
    dialog.buildGenreForm();

    if (dialog.exec() == QDialog::Accepted) {
        genre = dialog.getValue(dialog.name_key);
        return true;
    }

    return false;
}

PackagesDialog::PackagesDialog(ISource * currApi, QWidget * parent)
    : BaseDialog(parent), ui(new Ui::PackagesDialog) {

    ui -> setupUi(this);

    QMap<QString, QString> customs = currApi -> setsList();

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
    name = item -> text();
    params = item -> data(Qt::UserRole).toString();

    if (params.endsWith(QUrl::toPercentEncoding(PACKAGE_REPLACE_FRAGMENT))) {
        QString manual_genre;

        if (!takeGenre(manual_genre) || manual_genre.isEmpty()) return;

        params.replace(QUrl::toPercentEncoding(PACKAGE_REPLACE_FRAGMENT), manual_genre.toLower());
        name.replace(PACKAGE_REPLACE_FRAGMENT_TITLE, manual_genre);
    }

    accept();
}
