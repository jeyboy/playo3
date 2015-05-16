#include "search_dialog.h"
#include "ui_search_dialog.h"
#include "dockbars.h"

#include <QFileInfoList>

SearchDialog::SearchDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::SearchDialog)
{
    ui -> setupUi(this);

    QList<DockBar *> bars = Dockbars::instance() -> dockbars();
    QList<DockBar *>::Iterator it = bars.begin();

    for(; it != bars.end(); it++) {
        IView * v = Dockbars::instance() -> view(*it);
        if (v) {
            IModel * mdl = (IModel *)v -> model();
            if (mdl -> containerType() != search) {
                QListWidgetItem * item = new QListWidgetItem((*it) -> windowTitle(), ui -> tabsList);
                item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
                item -> setCheckState(Qt::Unchecked);
                item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) mdl));
                ui -> tabsList -> addItem(item);
            }
        }
    }

    QStringList genres = MusicGenres::instance() -> genresList();   genres.sort();
    ui -> stylePredicate -> addItems(genres);


    QFileInfoList drives = QDir::drives();
    for(QFileInfoList::Iterator it = drives.begin(); it != drives.end(); it++) {
        QListWidgetItem * item = new QListWidgetItem((*it).absolutePath(), ui -> driveList);
        item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
        item -> setCheckState(Qt::Unchecked);
        ui -> driveList -> addItem(item);
    }
}

SearchDialog::~SearchDialog() {
    delete ui;
}

SearchSettings SearchDialog::params() {
    SearchSettings res(ui -> inVk -> isChecked(), ui -> inSc -> isChecked(), ui -> inOther -> isChecked(),
                       ui -> inTabs -> isChecked(), ui -> inComputer -> isChecked());

    int count = ui -> textPredicates -> count();
    for(int i = 0; i < count; i++)
        res.predicates.append(ui -> textPredicates -> item(i) -> text());

    count = ui -> stylePredicates -> count();
    for(int i = 0; i < count; i++)
        res.addGenre(ui -> stylePredicates -> item(i) -> text());

    if (ui -> byTitle -> isChecked())
        res.type = ::title;
    else if (ui -> byArtist -> isChecked())
        res.type = ::artist;
    else if (ui -> bySong -> isChecked())
        res.type = ::song;
    else if (ui -> byStyle -> isChecked())
        res.type = ::style;
    else
        res.type = ::tag;

    res.popular = ui -> byPopular -> isChecked();

    if (res.inTabs) {
        int count = ui -> tabsList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = ui -> tabsList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.tabs.append(item -> data(Qt::UserRole + 1).value<void *>());
        }
    }

    if (res.inComputer) {
        int count = ui -> driveList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = ui -> driveList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.drives.append(item -> text());
        }
    }

    return res;
}

void SearchDialog::on_addPredicate_clicked() {
    QString predicate = ui -> textPredicate -> text();

    if (!predicate.isEmpty()) {
        if (ui -> textPredicates -> findItems(predicate, Qt::MatchFixedString).size() == 0)
            ui -> textPredicates -> addItem(predicate);
    }
}

void SearchDialog::on_cancelButton_clicked() {
    reject();
}

void SearchDialog::on_acceptButton_clicked() {
    accept();
}

void SearchDialog::on_textPredicates_itemActivated(QListWidgetItem * item) {
    ui -> textPredicates -> removeItemWidget(item);
    delete item;
}

void SearchDialog::on_stylePredicates_itemActivated(QListWidgetItem * item) {
    ui -> stylePredicates -> removeItemWidget(item);
    delete item;
}

void SearchDialog::on_addStylePredicate_clicked() {
    QString predicate = ui -> stylePredicate -> currentText();

    if (!predicate.isEmpty()) {
        if (ui -> stylePredicates -> findItems(predicate, Qt::MatchFixedString).size() == 0)
            ui -> stylePredicates -> addItem(predicate);
    }
}

void SearchDialog::on_driveList_itemClicked(QListWidgetItem * item) {
    if (item -> checkState() == Qt::Checked) {
        ui -> inComputer -> blockSignals(true);
        ui -> inComputer -> setChecked(true);
        ui -> inComputer -> blockSignals(false);
    }
}

void SearchDialog::on_inComputer_toggled(bool checked) {
    int count = ui -> driveList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = ui -> driveList -> item(i);
        item -> setCheckState(st);
    }
}

void SearchDialog::on_inTabs_toggled(bool checked) {
    int count = ui -> tabsList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = ui -> tabsList -> item(i);
        item -> setCheckState(st);
    }
}

void SearchDialog::on_tabsList_itemClicked(QListWidgetItem * item) {
    if (item -> checkState() == Qt::Checked) {
        ui -> inTabs -> blockSignals(true);
        ui -> inTabs -> setChecked(true);
        ui -> inTabs -> blockSignals(false);
    }
}
