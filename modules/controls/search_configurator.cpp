#include "search_configurator.h"

#include <qfileinfo.h>

#include "modules/core/web/web_apis.h"
#include "dockbars.h"

using namespace Controls;

void SearchConfigurator::initUI() {
    predicatesArea = new QScrollArea(this);
    predicatesArea->setObjectName(QStringLiteral("predicatesArea"));
    predicatesArea->setGeometry(QRect(10, 10, 451, 321));
    predicatesArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    predicatesArea->setWidgetResizable(false);
    predicatesAreaBody = new QWidget();
    predicatesAreaBody->setObjectName(QStringLiteral("predicatesAreaBody"));
    predicatesAreaBody->setGeometry(QRect(0, 0, 449, 319));
    textPredicates = new QListWidget(predicatesAreaBody);
    textPredicates->setObjectName(QStringLiteral("textPredicates"));
    textPredicates->setGeometry(QRect(10, 60, 211, 251));
    addStylePredicate = new QPushButton(predicatesAreaBody);
    addStylePredicate->setObjectName(QStringLiteral("addStylePredicate"));
    addStylePredicate->setGeometry(QRect(420, 29, 21, 24));
    addStylePredicate->setText(QApplication::translate("SearchDialog", "+", 0));

    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    font.setWeight(75);
    addStylePredicate->setFont(font);
    addPredicate = new QPushButton(predicatesAreaBody);
    addPredicate->setObjectName(QStringLiteral("addPredicate"));
    addPredicate->setGeometry(QRect(200, 29, 21, 24));
    addPredicate->setText(QApplication::translate("SearchDialog", "+", 0));

    QFont font1;
    font1.setPointSize(10);
    font1.setBold(true);
    font1.setItalic(false);
    font1.setWeight(75);
    font1.setStyleStrategy(QFont::PreferAntialias);
    addPredicate->setFont(font1);

    QLabel * sentencesLabel = new QLabel(predicatesAreaBody);
    sentencesLabel->setObjectName(QStringLiteral("sentencesLabel"));
    sentencesLabel->setGeometry(QRect(90, 9, 61, 20));
    QFont font2;
    font2.setBold(true);
    font2.setWeight(75);
    sentencesLabel->setFont(font2);
    sentencesLabel->setText(QApplication::translate("SearchDialog", "Sentences", 0));

    stylePredicate = new QComboBox(predicatesAreaBody);
    stylePredicate->setObjectName(QStringLiteral("stylePredicate"));
    stylePredicate->setGeometry(QRect(230, 30, 191, 22));

    stylePredicates = new QListWidget(predicatesAreaBody);
    stylePredicates->setObjectName(QStringLiteral("stylePredicates"));
    stylePredicates->setGeometry(QRect(230, 60, 211, 251));
    stylePredicates->setToolTip(QApplication::translate("SearchDialog", "Double click is removing item", 0));

    textPredicate = new QLineEdit(predicatesAreaBody);
    textPredicate->setObjectName(QStringLiteral("textPredicate"));
    textPredicate->setGeometry(QRect(10, 30, 191, 22));
    textPredicates->setToolTip(QApplication::translate("SearchDialog", "Double click is removing item", 0));

    QLabel * genresLabel = new QLabel(predicatesAreaBody);
    genresLabel->setObjectName(QStringLiteral("genresLabel"));
    genresLabel->setGeometry(QRect(320, 9, 51, 20));
    genresLabel->setFont(font2);
    genresLabel->setText(QApplication::translate("SearchDialog", "Genres", 0));

    predicatesArea->setWidget(predicatesAreaBody);

    locationsArea = new QScrollArea(this);
    locationsArea->setObjectName(QStringLiteral("locationsArea"));
    locationsArea->setGeometry(QRect(490, 10, 451, 321));
    locationsArea->setWidgetResizable(false);
    locationsAreaBody = new QWidget();
    locationsAreaBody->setObjectName(QStringLiteral("locationsAreaBody"));
    locationsAreaBody->setGeometry(QRect(0, 0, 449, 319));
    sitesList = new QListWidget(locationsAreaBody);
    sitesList->setObjectName(QStringLiteral("sitesList"));
    sitesList->setGeometry(QRect(10, 190, 431, 121));
    inSites = new QCheckBox(locationsAreaBody);
    inSites->setObjectName(QStringLiteral("inSites"));
    inSites->setGeometry(QRect(10, 170, 121, 20));
    inSites->setFont(font2);
    inSites->setToolTip(QApplication::translate("SearchDialog", "Search in VKontakte", 0));
    inSites->setText(QApplication::translate("SearchDialog", "Search in Sites", 0));

    tabsList = new QListWidget(locationsAreaBody);
    tabsList->setObjectName(QStringLiteral("tabsList"));
    tabsList->setGeometry(QRect(190, 30, 251, 121));
    driveList = new QListWidget(locationsAreaBody);
    driveList->setObjectName(QStringLiteral("driveList"));
    driveList->setGeometry(QRect(10, 30, 171, 121));
    inComputer = new QCheckBox(locationsAreaBody);
    inComputer->setObjectName(QStringLiteral("inComputer"));
    inComputer->setGeometry(QRect(10, 10, 141, 21));
    inComputer->setFont(font2);
    inComputer->setToolTip(QApplication::translate("SearchDialog", "Search in VKontakte", 0));
    inComputer->setText(QApplication::translate("SearchDialog", "Search in Computer", 0));

    inTabs = new QCheckBox(locationsAreaBody);
    inTabs->setObjectName(QStringLiteral("inTabs"));
    inTabs->setGeometry(QRect(190, 10, 121, 20));
    inTabs->setFont(font2);
    inTabs->setToolTip(QApplication::translate("SearchDialog", "Search in VKontakte", 0));
    inTabs->setText(QApplication::translate("SearchDialog", "Search in Tabs", 0));

    locationsArea->setWidget(locationsAreaBody);

    limitationsArea = new QScrollArea(this);
    limitationsArea->setObjectName(QStringLiteral("limitationsArea"));
    limitationsArea->setGeometry(QRect(10, 350, 451, 171));
    limitationsArea->setWidgetResizable(false);
    limitationsAreaBody = new QWidget();
    limitationsAreaBody->setObjectName(QStringLiteral("limitationsAreaBody"));
    limitationsAreaBody->setGeometry(QRect(0, 0, 448, 169));
    searchInGroup = new QGroupBox(limitationsAreaBody);
    searchInGroup->setObjectName(QStringLiteral("searchInGroup"));
    searchInGroup->setGeometry(QRect(10, 100, 431, 61));
    searchInGroup->setTitle(QApplication::translate("SearchDialog", "Search In", 0));

    byPopular = new QCheckBox(searchInGroup);
    byPopular->setObjectName(QStringLiteral("byPopular"));
    byPopular->setGeometry(QRect(10, 16, 101, 17));
    byPopular->setFont(font2);
    byPopular->setText(QApplication::translate("SearchDialog", "In popular", 0));

    byOwns = new QCheckBox(searchInGroup);
    byOwns->setObjectName(QStringLiteral("byOwns"));
    byOwns->setGeometry(QRect(10, 40, 101, 17));
    byOwns->setFont(font2);
    byOwns->setText(QApplication::translate("SearchDialog", "In owns", 0));

    byOrigins = new QCheckBox(searchInGroup);
    byOrigins->setObjectName(QStringLiteral("byOrigins"));
    byOrigins->setGeometry(QRect(150, 20, 121, 17));
    byOrigins->setFont(font2);
    byOrigins->setText(QApplication::translate("SearchDialog", "In originals only", 0));

    byForeign = new QCheckBox(searchInGroup);
    byForeign->setObjectName(QStringLiteral("byForeign"));
    byForeign->setGeometry(QRect(150, 40, 111, 17));
    byForeign->setFont(font2);
    byForeign->setText(QApplication::translate("SearchDialog", "In foreign", 0));

    searchByGroup = new QGroupBox(limitationsAreaBody);
    searchByGroup->setObjectName(QStringLiteral("searchByGroup"));
    searchByGroup->setGeometry(QRect(10, 10, 431, 81));
    byTitle = new QRadioButton(searchByGroup);
    byTitle->setObjectName(QStringLiteral("byTitle"));
    byTitle->setGeometry(QRect(10, 20, 51, 17));
    byTitle->setFont(font2);
    byTitle->setChecked(true);
    byTag = new QRadioButton(searchByGroup);
    byTag->setObjectName(QStringLiteral("byTag"));
    byTag->setEnabled(false);
    byTag->setGeometry(QRect(220, 20, 51, 17));
    byTag->setFont(font2);
    byArtist = new QRadioButton(searchByGroup);
    byArtist->setObjectName(QStringLiteral("byArtist"));
    byArtist->setGeometry(QRect(30, 40, 161, 17));
    byArtist->setFont(font2);
    byArtist->setChecked(false);
    bySong = new QRadioButton(searchByGroup);
    bySong->setObjectName(QStringLiteral("bySong"));
    bySong->setGeometry(QRect(30, 60, 161, 17));
    bySong->setFont(font2);
    bySong->setChecked(false);

    searchByGroup->setTitle(QApplication::translate("SearchDialog", "Search By", 0));
    byTitle->setText(QApplication::translate("SearchDialog", "Title", 0));
    byTag->setText(QApplication::translate("SearchDialog", "Tag", 0));
    byArtist->setText(QApplication::translate("SearchDialog", "by artist if possible", 0));
    bySong->setText(QApplication::translate("SearchDialog", "by song title if possible", 0));

    limitationsArea->setWidget(limitationsAreaBody);
}

void SearchConfigurator::initiateSources() {
    QList<DockBar *> bars = Presentation::Dockbars::obj().dockbars();
    for(QList<DockBar *>::Iterator it = bars.begin(); it != bars.end(); it++) {
        Views::IView * v = Presentation::Dockbars::obj().view(*it);
        if (v) {
            Models::IModel * mdl = (Models::IModel *)v -> model();
            if (mdl -> playlistType() != Data::search) {
                QListWidgetItem * item = new QListWidgetItem((*it) -> windowTitle(), tabsList);
                item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
                item -> setCheckState(Qt::Unchecked);
                item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) mdl));
                tabsList -> addItem(item);
            }
        }
    }

    QHash<Core::Web::SubType, Core::ISearchable *> sites = Core::Web::Apis::list();
    QListWidgetItem * item = new QListWidgetItem(QString(""));
    item -> setFlags(Qt::NoItemFlags);
    sitesList -> addItem(item);

    for(QHash<Core::Web::SubType, Core::ISearchable *>::Iterator it = sites.begin(); it != sites.end(); it++) {
        if ((*it) -> isConnected()) {
            QListWidgetItem * item = new QListWidgetItem(it.value() -> name());
            item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
            item -> setCheckState(Qt::Unchecked);
            item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) it.value()));

            switch(it.key()) {
                case Core::Web::SubType::site_vk:
                case Core::Web::SubType::site_sc:
                case Core::Web::SubType::site_od:
                case Core::Web::SubType::site_fourshared: {
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

    if (res.inSites) {
        QList<Core::ISearchable *> searchables = Core::Web::Apis::list().values();
        for(QList<Core::ISearchable *>::Iterator it = searchables.begin(); it != searchables.end(); it++)
            res.sites.append(*it);
    }

    if (res.inTabs) {
        QList<Controls::DockBar *> dockbars = Presentation::Dockbars::obj().dockbars();
        for(QList<Controls::DockBar *>::Iterator it = dockbars.begin(); it != dockbars.end(); it++) {
            Views::IView * v = Presentation::Dockbars::obj().view(*it);
            if (v) res.tabs.append((Models::IModel *)v -> model());
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
    initUI();
    initiateSources();

    setObjectName(QStringLiteral("searchConfigurator"));
    setExclusive(true);
    setToggleable(false);
    addItem(QStringLiteral("In locations"), locationsArea, true);
    addItem(QStringLiteral("By predicates"), predicatesArea);
    addItem(QStringLiteral("With limitations"), limitationsArea);

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
    else if (bySong -> isChecked())
        res.type = Core::ISearchable::in_song;
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
