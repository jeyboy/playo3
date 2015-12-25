#include "search_configurator.h"

#include <qfileinfo.h>

#include "modules/core/web/web_apis.h"
#include "dockbars.h"

using namespace Controls;

QScrollArea * SearchConfigurator::locationsArea() {

}
QScrollArea * SearchConfigurator::predicatesArea() {

}
QScrollArea * SearchConfigurator::limitationsArea() {

}

void SearchConfigurator::initiateSources() {
    QList<DockBar *> bars = Dockbars::obj().dockbars();
    for(QList<DockBar *>::Iterator it = bars.begin(); it != bars.end(); it++) {
        IView * v = Dockbars::obj().view(*it);
        if (v) {
            IModel * mdl = (IModel *)v -> model();
            if (mdl -> playlistType() != Data::search) {
                QListWidgetItem * item = new QListWidgetItem((*it) -> windowTitle(), ui -> tabsList);
                item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
                item -> setCheckState(Qt::Unchecked);
                item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) mdl));
                ui -> tabsList -> addItem(item);
            }
        }
    }

    QHash<Web::SubType, ISearchable *> sites = Web::Apis::list();
    QListWidgetItem * item = new QListWidgetItem(QString(""));
    item -> setFlags(Qt::NoItemFlags);
    ui -> sitesList -> addItem(item);

    bool has_not_connected = false;

    for(QHash<Web::SubType, ISearchable *>::Iterator it = sites.begin(); it != sites.end(); it++) {
        if ((*it) -> isConnected()) {
            QListWidgetItem * item = new QListWidgetItem(it.value() -> name());
            item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
            item -> setCheckState(Qt::Unchecked);
            item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) it.value()));

            switch(it.key()) {
                case Web::SubType::site_vk:
                case Web::SubType::site_sc:
                case Web::SubType::site_od:
                case Web::SubType::site_fourshared: {
                    ui -> sitesList -> insertItem(0, item);
                break;}
                default: ui -> sitesList -> addItem(item);
            }
        } else has_not_connected = true;
    }

    if (has_not_connected)
         setWindowTitle(windowTitle() % QStringLiteral(" (some search services is not connected)"));

    QStringList genres = MusicGenres::obj().genresList();   genres.sort();
    ui -> stylePredicate -> addItems(genres);

    QFileInfoList drives = QDir::drives();
    for(QFileInfoList::Iterator it = drives.begin(); it != drives.end(); it++) {
        QListWidgetItem * item = new QListWidgetItem((*it).absolutePath(), ui -> driveList);
        item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
        item -> setCheckState(Qt::Unchecked);
        ui -> driveList -> addItem(item);
    }
}

SearchConfigurator::SearchConfigurator(QWidget * parent, QPushButton * activationBtn) : Accordion(parent), activationBtn(activationBtn) {
    setExclusive(true);
    setToggleable(false);
    addItem(QStringLiteral("In locations"), locationsArea(), true);
    addItem(QStringLiteral("By predicates"), predicatesArea());
    addItem(QStringLiteral("With limitations"), limitationsArea());

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(on_accordeon_currentChanged(int)));
}

SearchSettings SearchConfigurator::params() {
    SearchSettings res(ui -> inSites -> isChecked(), ui -> inTabs -> isChecked(), ui -> inComputer -> isChecked());

    int count = ui -> textPredicates -> count();
    for(int i = 0; i < count; i++)
        res.predicates.append(ui -> textPredicates -> item(i) -> text());

    count = ui -> stylePredicates -> count();
    for(int i = 0; i < count; i++)
        res.addGenre(ui -> stylePredicates -> item(i) -> text());

    if (ui -> byTitle -> isChecked())
        res.type = ISearchable::in_title;
    else if (ui -> byArtist -> isChecked())
        res.type = ISearchable::in_artist;
    else if (ui -> bySong -> isChecked())
        res.type = ISearchable::in_song;
    else
        res.type = ISearchable::in_tag;

    if (ui -> byPopular -> isChecked()) res.type |= ISearchable::in_popular;
    if (ui -> byOwns -> isChecked()) res.type |= ISearchable::in_owns;
    if (ui -> byForeign -> isChecked()) res.type |= ISearchable::in_foreign;
    if (ui -> byOrigins -> isChecked()) res.type |= ISearchable::in_originals;


    if (res.inSites) {
        int count = ui -> sitesList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = ui -> sitesList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.sites.append(item -> data(Qt::UserRole + 1).value<void *>());
        }
    }

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

void SearchConfigurator::on_addPredicate_clicked() {
    QString predicate = ui -> textPredicate -> text();

    if (!predicate.isEmpty()) {
        if (ui -> textPredicates -> findItems(predicate, Qt::MatchFixedString).size() == 0) {
            ui -> textPredicates -> addItem(predicate);
            ui -> textPredicate -> setText("");
        }
    }
}

void SearchConfigurator::on_textPredicates_itemActivated(QListWidgetItem * item) {
    ui -> textPredicates -> removeItemWidget(item);
    delete item;
}

void SearchConfigurator::on_stylePredicates_itemActivated(QListWidgetItem * item) {
    ui -> stylePredicates -> removeItemWidget(item);
    delete item;
}

void SearchConfigurator::on_addStylePredicate_clicked() {
    QString predicate = ui -> stylePredicate -> currentText();

    if (!predicate.isEmpty()) {
        if (ui -> stylePredicates -> findItems(predicate, Qt::MatchFixedString).size() == 0)
            ui -> stylePredicates -> addItem(predicate);
    }
}

void SearchConfigurator::on_driveList_itemClicked(QListWidgetItem * item) {
    bool st = item -> checkState() == Qt::Checked;

    if (!st) {
        int count = ui -> driveList -> count();

        for(int i = 0; i < count; i++)
            st |= ui -> driveList -> item(i) -> checkState() == Qt::Checked;
    }

    ui -> inComputer -> blockSignals(true);
    ui -> inComputer -> setChecked(st);
    ui -> inComputer -> blockSignals(false);
}

void SearchConfigurator::on_inComputer_toggled(bool checked) {
    int count = ui -> driveList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = ui -> driveList -> item(i);
        item -> setCheckState(st);
    }
}

void SearchConfigurator::on_inTabs_toggled(bool checked) {
    int count = ui -> tabsList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = ui -> tabsList -> item(i);
        item -> setCheckState(st);
    }
}

void SearchConfigurator::on_tabsList_itemClicked(QListWidgetItem * item) {
    bool st = item -> checkState() == Qt::Checked;

    if (!st) {
        int count = ui -> tabsList -> count();

        for(int i = 0; i < count; i++)
            st |= ui -> tabsList -> item(i) -> checkState() == Qt::Checked;
    }

    ui -> inTabs -> blockSignals(true);
    ui -> inTabs -> setChecked(st);
    ui -> inTabs -> blockSignals(false);
}

void SearchConfigurator::on_inSites_toggled(bool checked) {
    int count = ui -> sitesList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = ui -> sitesList -> item(i);
        if (item -> flags() & Qt::ItemIsUserCheckable)
            item -> setCheckState(st);
    }
}

void SearchConfigurator::on_sitesList_itemClicked(QListWidgetItem * item) {
    bool st = item -> checkState() == Qt::Checked;

    if (!st) {
        int count = ui -> sitesList -> count();

        for(int i = 0; i < count; i++)
            st |= ui -> sitesList -> item(i) -> checkState() == Qt::Checked;
    }

    ui -> inSites -> blockSignals(true);
    ui -> inSites -> setChecked(st);
    ui -> inSites -> blockSignals(false);
}

void SearchConfigurator::on_accordeon_currentChanged(int index) {
    bool predicable = index == (int)ui -> predicatesArea;
    qDebug() << "CUU" << index << (int)ui -> predicatesArea;

    if (activationBtn) activationBtn -> setDefault(!predicable);
    addPredicate -> setDefault(predicable);
}
