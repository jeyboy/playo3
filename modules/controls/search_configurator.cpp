#include "search_configurator.h"

#include <qfileinfo.h>

#include "modules/core/web/web_apis.h"
#include "dockbars.h"

using namespace Core;
using namespace Controls;

QScrollArea * SearchConfigurator::initLocations() {
    QScrollArea * locationsArea = new QScrollArea(this);
    locationsArea -> setObjectName(QStringLiteral("locationsArea"));
//    locationsArea -> setWidgetResizable(false);

    QWidget * locationsAreaBody = new QWidget();
    locationsAreaBody -> setObjectName(QStringLiteral("locationsAreaBody"));

    QGridLayout * l = new QGridLayout(locationsAreaBody);

    inSites = new QCheckBox(locationsAreaBody);
    inSites -> setObjectName(QStringLiteral("inSites"));
    inSites -> setText(QApplication::translate("SearchDialog", "Search in Sites", 0));
    inSites -> setStyleSheet("font-weight: bold;");
    l -> addWidget(inSites, 0, 0);

    sitesList = new QListWidget(locationsAreaBody);
    sitesList -> setObjectName(QStringLiteral("sitesList"));
    l -> addWidget(sitesList, 1, 0, 3, 1);


    inComputer = new QCheckBox(locationsAreaBody);
    inComputer -> setObjectName(QStringLiteral("inComputer"));
    inComputer -> setText(QApplication::translate("SearchDialog", "Search in Computer", 0));
    inComputer -> setStyleSheet("font-weight: bold;");
    l -> addWidget(inComputer, 0, 1);

    driveList = new QListWidget(locationsAreaBody);
    driveList -> setObjectName(QStringLiteral("driveList"));
    l -> addWidget(driveList, 1, 1);


    inTabs = new QCheckBox(locationsAreaBody);
    inTabs -> setObjectName(QStringLiteral("inTabs"));
    inTabs -> setText(QApplication::translate("SearchDialog", "Search in Tabs", 0));
    inTabs -> setStyleSheet("font-weight: bold;");
    l -> addWidget(inTabs, 2, 1);

    tabsList = new QListWidget(locationsAreaBody);
    tabsList -> setObjectName(QStringLiteral("tabsList"));
    l -> addWidget(tabsList, 3, 1);

    locationsArea -> setWidget(locationsAreaBody);
    return locationsArea;
}

QScrollArea * SearchConfigurator::initPredicates() {
    predicatesArea = new QScrollArea(this);
    predicatesArea -> setObjectName(QStringLiteral("predicatesArea"));
//    predicatesArea->setWidgetResizable(false);

    QWidget * predicatesAreaBody = new QWidget();
    predicatesAreaBody -> setObjectName(QStringLiteral("predicatesAreaBody"));

    QGridLayout * l = new QGridLayout(predicatesAreaBody);

    QLabel * sentencesLabel = new QLabel(predicatesAreaBody);
    sentencesLabel -> setObjectName(QStringLiteral("sentencesLabel"));
    sentencesLabel -> setText(QApplication::translate("SearchDialog", "Sentences", 0));
    l -> addWidget(sentencesLabel, 0, 0, 1, 2, Qt::AlignCenter);


    QLabel * genresLabel = new QLabel(predicatesAreaBody);
    genresLabel -> setObjectName(QStringLiteral("genresLabel"));
    genresLabel -> setText(QApplication::translate("SearchDialog", "Genres", 0));
    l -> addWidget(genresLabel, 0, 2, 1, 2, Qt::AlignCenter);


    textPredicate = new QLineEdit(predicatesAreaBody);
    textPredicate -> setObjectName(QStringLiteral("textPredicate"));
    l -> addWidget(textPredicate, 1, 0);

    addPredicate = new QPushButton(predicatesAreaBody);
    addPredicate -> setObjectName(QStringLiteral("addPredicate"));
    addPredicate -> setText(QApplication::translate("SearchDialog", "+", 0));
    addPredicate -> setMaximumSize(24, 32);
    l -> addWidget(addPredicate, 1, 1);

    textPredicates = new QListWidget(predicatesAreaBody);
    textPredicates -> setObjectName(QStringLiteral("textPredicates"));
    textPredicates -> setToolTip(QApplication::translate("SearchDialog", "Double click is removing item", 0));
    l -> addWidget(textPredicates, 2, 0, 1, 2);


    stylePredicate = new QComboBox(predicatesAreaBody);
    stylePredicate -> setObjectName(QStringLiteral("stylePredicate"));
    l -> addWidget(stylePredicate, 1, 2);

    addStylePredicate = new QPushButton(predicatesAreaBody);
    addStylePredicate -> setObjectName(QStringLiteral("addStylePredicate"));
    addStylePredicate -> setText(QApplication::translate("SearchDialog", "+", 0));
    addStylePredicate -> setMaximumSize(24, 32);
    l -> addWidget(addStylePredicate, 1, 3);

    stylePredicates = new QListWidget(predicatesAreaBody);
    stylePredicates -> setObjectName(QStringLiteral("stylePredicates"));
    stylePredicates -> setToolTip(QApplication::translate("SearchDialog", "Double click is removing item", 0));
    l -> addWidget(stylePredicates, 2, 2, 1, 2);


    predicatesArea -> setWidget(predicatesAreaBody);
    return predicatesArea;
}

QScrollArea * SearchConfigurator::initLimitations() {
    QScrollArea * limitationsArea = new QScrollArea(this);
    limitationsArea -> setObjectName(QStringLiteral("limitationsArea"));
//    limitationsArea->setWidgetResizable(false);

    QWidget * limitationsAreaBody = new QWidget();
    limitationsAreaBody -> setObjectName(QStringLiteral("limitationsAreaBody"));

    QVBoxLayout * l = new QVBoxLayout(limitationsAreaBody);


    QGroupBox * searchByGroup = new QGroupBox(limitationsAreaBody);
    searchByGroup -> setObjectName(QStringLiteral("searchByGroup"));
    searchByGroup -> setTitle(QApplication::translate("SearchDialog", "Search By", 0));

    l -> addWidget(searchByGroup, 1);
    QGridLayout * gl1 = new QGridLayout(searchByGroup);

    byTitle = new QRadioButton(searchByGroup);
    byTitle -> setObjectName(QStringLiteral("byTitle"));
    byTitle -> setChecked(true);
    byTitle -> setText(QApplication::translate("SearchDialog", "Title", 0));
    gl1 -> addWidget(byTitle, 0, 0, 1, 2);

    byArtist = new QRadioButton(searchByGroup);
    byArtist -> setObjectName(QStringLiteral("byArtist"));
    byArtist -> setChecked(false);
    byArtist -> setText(QApplication::translate("SearchDialog", "by artist if possible", 0));
    gl1 -> addWidget(byArtist, 1, 1);

    bySongName = new QRadioButton(searchByGroup);
    bySongName -> setObjectName(QStringLiteral("bySong"));
    bySongName -> setChecked(false);
    bySongName -> setText(QApplication::translate("SearchDialog", "by song title if possible", 0));
    gl1 -> addWidget(bySongName, 2, 1);

    byTag = new QRadioButton(searchByGroup);
    byTag -> setObjectName(QStringLiteral("byTag"));
    byTag -> setEnabled(false);
    byTag -> setText(QApplication::translate("SearchDialog", "Tag", 0));
    gl1 -> addWidget(byTag, 0, 2);



    QGroupBox * searchInGroup = new QGroupBox(limitationsAreaBody);
    searchInGroup -> setObjectName(QStringLiteral("searchInGroup"));
    searchInGroup -> setTitle(QApplication::translate("SearchDialog", "Search In", 0));

    l -> addWidget(searchInGroup, 1);
    QGridLayout * gl2 = new QGridLayout(searchInGroup);

    byPopular = new QCheckBox(searchInGroup);
    byPopular -> setObjectName(QStringLiteral("byPopular"));
    byPopular -> setText(QApplication::translate("SearchDialog", "In popular", 0));
    gl2 -> addWidget(byPopular, 0, 0);

    byOwns = new QCheckBox(searchInGroup);
    byOwns -> setObjectName(QStringLiteral("byOwns"));
    byOwns -> setText(QApplication::translate("SearchDialog", "In owns", 0));
    gl2 -> addWidget(byOwns, 1, 0);

    byOrigins = new QCheckBox(searchInGroup);
    byOrigins -> setObjectName(QStringLiteral("byOrigins"));
    byOrigins -> setText(QApplication::translate("SearchDialog", "In originals only", 0));
    gl2 -> addWidget(byOrigins, 0, 1);

    byForeign = new QCheckBox(searchInGroup);
    byForeign -> setObjectName(QStringLiteral("byForeign"));
    byForeign -> setText(QApplication::translate("SearchDialog", "In foreign", 0));
    gl2 -> addWidget(byForeign, 1, 1);


    limitationsArea -> setWidget(limitationsAreaBody);
    return limitationsArea;
}

void SearchConfigurator::initiateSources() {
    QList<DockBar *> bars = Presentation::Dockbars::obj().dockbars();
    for(QList<DockBar *>::Iterator it = bars.begin(); it != bars.end(); it++) {
        Views::IView * v = Presentation::Dockbars::obj().view(*it);
        if (v && !v -> isCommon()) {
            Models::IModel * mdl = (Models::IModel *)v -> model();
            if (mdl -> playlistType() != dt_search) {
                QListWidgetItem * item = new QListWidgetItem((*it) -> windowTitle(), tabsList);
                item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
                item -> setCheckState(Qt::Unchecked);
                item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) mdl));
                tabsList -> addItem(item);
            }
        }
    }

    QHash<DataSubType, ISearchable *> sites = Web::Apis::searchersList();
    QListWidgetItem * item = new QListWidgetItem(QString(""));
    item -> setFlags(Qt::NoItemFlags);
    sitesList -> addItem(item);

    for(QHash<DataSubType, ISearchable *>::Iterator it = sites.begin(); it != sites.end(); it++) {
        ISource * src = Web::Apis::source(it.key());

        if (src -> isConnected()) {
            QListWidgetItem * item = new QListWidgetItem(src -> name());
            item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
            item -> setCheckState(Qt::Unchecked);
            item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) it.value()));

            switch(it.key()) {
                case dt_site_vk:
                case dt_site_sc:
                case dt_site_od:
                case dt_site_fourshared: {
                    sitesList -> insertItem(0, item);
                break;}
                default: sitesList -> addItem(item);
            }
        } else has_not_connected = true;
    }

    if (has_not_connected)
         setWindowTitle(windowTitle() % QStringLiteral(" (some search services is not connected)"));

    QStringList genres = MusicGenres::obj().genresList();   genres.sort();
    stylePredicate -> addItems(genres);

    QFileInfoList drives = QDir::drives();
    for(QFileInfoList::Iterator it = drives.begin(); it != drives.end(); it++) {
        QListWidgetItem * item = new QListWidgetItem((*it).absolutePath(), driveList);
        item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
        item -> setCheckState(Qt::Unchecked);
        driveList -> addItem(item);
    }
}

SearchSettings SearchConfigurator::buildParams(int limitPerPredicate, const SearchSettingsBlocks & blocks, const QStringList & predicates, const QStringList & genres) {
    SearchSettings res(blocks & block_sites, blocks & block_tabs, blocks & block_computer, limitPerPredicate);

    res.predicates.append(predicates);
    res.genres.append(genres);
    res.type = Core::ISearchable::in_title;

    if (res.inSites) {
        QList<Core::ISearchable *> searchables = Core::Web::Apis::searchersList().values();
        for(QList<Core::ISearchable *>::Iterator it = searchables.begin(); it != searchables.end(); it++)
            if (Web::Apis::source((*it) -> siteType()) -> isConnected())
                res.sites.append(*it);
    }

    if (res.inTabs) {
        QList<Controls::DockBar *> dockbars = Presentation::Dockbars::obj().dockbars();
        for(QList<Controls::DockBar *>::Iterator it = dockbars.begin(); it != dockbars.end(); it++) {
            Views::IView * v = Presentation::Dockbars::obj().view(*it);
            if (v && !v -> isCommon()) res.tabs.append((Models::IModel *)v -> model());
        }
    }

    if (res.inComputer) {
        QFileInfoList drives = QDir::drives();
        for(QFileInfoList::Iterator it = drives.begin(); it != drives.end(); it++)
            res.drives.append((*it).absolutePath());
    }

    return res;
}

SearchConfigurator::SearchConfigurator(QWidget * parent, QPushButton * activationBtn) : Accordion(parent), activationBtn(activationBtn), has_not_connected(false) {

    setObjectName(QStringLiteral("searchConfigurator"));
    setExclusive(true);
    setToggleable(false);
    addItem(QStringLiteral("In locations"), initLocations(), true);
    addItem(QStringLiteral("By predicates"), initPredicates());
    addItem(QStringLiteral("With limitations"), initLimitations());

    initiateSources();

    QMetaObject::connectSlotsByName(this);
}

SearchSettings SearchConfigurator::params() {
    SearchSettings res(inSites -> isChecked(), inTabs -> isChecked(), inComputer -> isChecked());

    int count = textPredicates -> count();
    for(int i = 0; i < count; i++)
        res.predicates.append(textPredicates -> item(i) -> text());

    count = stylePredicates -> count();
    for(int i = 0; i < count; i++)
        res.genres.append(stylePredicates -> item(i) -> text());

    if (byTitle -> isChecked())
        res.type = Core::ISearchable::in_title;
    else if (byArtist -> isChecked())
        res.type = Core::ISearchable::in_artist;
    else if (bySongName -> isChecked())
        res.type = Core::ISearchable::in_song_name;
    else
        res.type = Core::ISearchable::in_tag;

    if (byPopular -> isChecked()) res.type |= Core::ISearchable::in_popular;
    if (byOwns -> isChecked()) res.type |= Core::ISearchable::in_owns;
    if (byForeign -> isChecked()) res.type |= Core::ISearchable::in_foreign;
    if (byOrigins -> isChecked()) res.type |= Core::ISearchable::in_originals;


    if (res.inSites) {
        int count = sitesList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = sitesList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.sites.append(item -> data(Qt::UserRole + 1).value<void *>());
        }
    }

    if (res.inTabs) {
        int count = tabsList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = tabsList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.tabs.append(item -> data(Qt::UserRole + 1).value<void *>());
        }
    }

    if (res.inComputer) {
        int count = driveList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = driveList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.drives.append(item -> text());
        }
    }

    return res;
}

void SearchConfigurator::on_addPredicate_clicked() {
    QString predicate = textPredicate -> text();

    if (!predicate.isEmpty())
        if (textPredicates -> findItems(predicate, Qt::MatchFixedString).size() == 0) {
            textPredicates -> addItem(predicate);
            textPredicate -> setText("");
        }
}

void SearchConfigurator::on_textPredicates_itemActivated(QListWidgetItem * item) {
    textPredicates -> removeItemWidget(item);
    delete item;
}

void SearchConfigurator::on_stylePredicates_itemActivated(QListWidgetItem * item) {
    stylePredicates -> removeItemWidget(item);
    delete item;
}

void SearchConfigurator::on_addStylePredicate_clicked() {
    QString predicate = stylePredicate -> currentText();

    if (!predicate.isEmpty()) {
        if (stylePredicates -> findItems(predicate, Qt::MatchFixedString).size() == 0)
            stylePredicates -> addItem(predicate);
    }
}

void SearchConfigurator::on_driveList_itemClicked(QListWidgetItem * item) {
    bool st = item -> checkState() == Qt::Checked;

    if (!st) {
        int count = driveList -> count();

        for(int i = 0; i < count; i++)
            st |= driveList -> item(i) -> checkState() == Qt::Checked;
    }

    inComputer -> blockSignals(true);
    inComputer -> setChecked(st);
    inComputer -> blockSignals(false);
}

void SearchConfigurator::on_inComputer_toggled(bool checked) {
    int count = driveList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = driveList -> item(i);
        item -> setCheckState(st);
    }
}

void SearchConfigurator::on_inTabs_toggled(bool checked) {
    int count = tabsList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = tabsList -> item(i);
        item -> setCheckState(st);
    }
}

void SearchConfigurator::on_tabsList_itemClicked(QListWidgetItem * item) {
    bool st = item -> checkState() == Qt::Checked;

    if (!st) {
        int count = tabsList -> count();

        for(int i = 0; i < count; i++)
            st |= tabsList -> item(i) -> checkState() == Qt::Checked;
    }

    inTabs -> blockSignals(true);
    inTabs -> setChecked(st);
    inTabs -> blockSignals(false);
}

void SearchConfigurator::on_inSites_toggled(bool checked) {
    int count = sitesList -> count();

    Qt::CheckState st = checked ? Qt::Checked : Qt::Unchecked;

    for(int i = 0; i < count; i++) {
        QListWidgetItem * item = sitesList -> item(i);
        if (item -> flags() & Qt::ItemIsUserCheckable)
            item -> setCheckState(st);
    }
}

void SearchConfigurator::on_sitesList_itemClicked(QListWidgetItem * item) {
    bool st = item -> checkState() == Qt::Checked;

    if (!st) {
        int count = sitesList -> count();

        for(int i = 0; i < count; i++)
            st |= sitesList -> item(i) -> checkState() == Qt::Checked;
    }

    inSites -> blockSignals(true);
    inSites -> setChecked(st);
    inSites -> blockSignals(false);
}

void SearchConfigurator::on_searchConfigurator_currentChanged(int index) {
    bool predicable = index == (int)predicatesArea;

    if (activationBtn) activationBtn -> setDefault(!predicable);
    addPredicate -> setDefault(predicable);
}
