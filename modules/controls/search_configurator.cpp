#include "search_configurator.h"

#include <qfileinfo.h>

#include "modules/core/web/web_apis.h"
#include "dockbars.h"

using namespace Core;
using namespace Controls;

QWidget * SearchConfigurator::initLocations() {
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

    return locationsAreaBody;
}

QWidget * SearchConfigurator::initPredicates() {
    predicatesAreaBody = new QWidget();
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

    return predicatesAreaBody;
}

QWidget * SearchConfigurator::initLimitations() {
    QWidget * limitationsAreaBody = new QWidget();
    limitationsAreaBody -> setObjectName(QStringLiteral("limitationsAreaBody"));

    QVBoxLayout * l = new QVBoxLayout(limitationsAreaBody);

    QGroupBox * searchByTypesGroup = new QGroupBox(limitationsAreaBody);
    searchByTypesGroup -> setObjectName(QStringLiteral("searchByTypesGroup"));
    searchByTypesGroup -> setTitle(QApplication::translate("SearchDialog", "Search By Types", 0));

    l -> addWidget(searchByTypesGroup, 1);

    QHBoxLayout * hl = new QHBoxLayout(searchByTypesGroup);

    byAllTypes = new QRadioButton(searchByTypesGroup);
    byAllTypes -> setObjectName(QStringLiteral("byAllTypes"));
    byAllTypes -> setChecked(true);
    byAllTypes -> setText(QApplication::translate("SearchDialog", "by all types", 0));
    hl -> addWidget(byAllTypes, 1);

    byAudioTypes = new QRadioButton(searchByTypesGroup);
    byAudioTypes -> setObjectName(QStringLiteral("byAudioTypes"));
    byAudioTypes -> setChecked(false);
    byAudioTypes -> setText(QApplication::translate("SearchDialog", "by audio", 0));
    hl -> addWidget(byAudioTypes, 1);

    byVideoTypes = new QRadioButton(searchByTypesGroup);
    byVideoTypes -> setObjectName(QStringLiteral("byVideoTypes"));
    byVideoTypes -> setChecked(false);
    byVideoTypes -> setText(QApplication::translate("SearchDialog", "by video", 0));
    hl -> addWidget(byVideoTypes, 1);


    QGroupBox * searchByGroup = new QGroupBox(limitationsAreaBody);
    searchByGroup -> setObjectName(QStringLiteral("searchByGroup"));
    searchByGroup -> setTitle(QApplication::translate("SearchDialog", "Search By", 0));

    l -> addWidget(searchByGroup, 1);
    QGridLayout * gl1 = new QGridLayout(searchByGroup);

    byTitle = new QRadioButton(searchByGroup);
    byTitle -> setObjectName(QStringLiteral("byTitle"));
    byTitle -> setChecked(true);
    byTitle -> setText(QApplication::translate("SearchDialog", "Title", 0));
    gl1 -> addWidget(byTitle, 0, 0);

    byArtist = new QRadioButton(searchByGroup);
    byArtist -> setObjectName(QStringLiteral("byArtist"));
    byArtist -> setChecked(false);
    byArtist -> setText(QApplication::translate("SearchDialog", "by artist if possible", 0));
    gl1 -> addWidget(byArtist, 1, 0);

    bySongName = new QRadioButton(searchByGroup);
    bySongName -> setObjectName(QStringLiteral("bySong"));
    bySongName -> setChecked(false);
    bySongName -> setText(QApplication::translate("SearchDialog", "by song title if possible", 0));
    gl1 -> addWidget(bySongName, 2, 0);

    byTag = new QRadioButton(searchByGroup);
    byTag -> setObjectName(QStringLiteral("byTag"));
    byTag -> setText(QApplication::translate("SearchDialog", "Tag", 0));
    gl1 -> addWidget(byTag, 0, 1);

    bySet = new QRadioButton(searchByGroup);
    bySet -> setObjectName(QStringLiteral("bySet"));
    bySet -> setText(QApplication::translate("SearchDialog", "Set", 0));
    gl1 -> addWidget(bySet, 1, 1);


    byLyric = new QRadioButton(searchByGroup);
    byLyric -> setObjectName(QStringLiteral("byLyric"));
    byLyric -> setText(QApplication::translate("SearchDialog", "Lyric", 0));
    gl1 -> addWidget(byLyric, 0, 2);

    byAbc = new QRadioButton(searchByGroup);
    byAbc -> setObjectName(QStringLiteral("byAbc"));
    byAbc -> setText(QApplication::translate("SearchDialog", "Abc", 0));
    gl1 -> addWidget(byAbc, 1, 2);



    QGroupBox * searchInGroup = new QGroupBox(limitationsAreaBody);
    searchInGroup -> setObjectName(QStringLiteral("searchInGroup"));
    searchInGroup -> setTitle(QApplication::translate("SearchDialog", "Search In", 0));

    l -> addWidget(searchInGroup, 1);
    QGridLayout * gl2 = new QGridLayout(searchInGroup);



    byFresh = new QCheckBox(searchInGroup);
    byFresh -> setObjectName(QStringLiteral("byFresh"));
    byFresh -> setText(QApplication::translate("SearchDialog", "Fresh", 0));
    gl2 -> addWidget(byFresh, 0, 0);

    byNew = new QCheckBox(searchInGroup);
    byNew -> setObjectName(QStringLiteral("byNew"));
    byNew -> setText(QApplication::translate("SearchDialog", "New", 0));
    gl2 -> addWidget(byNew, 1, 0);

    byPopular = new QCheckBox(searchInGroup);
    byPopular -> setObjectName(QStringLiteral("byPopular"));
    byPopular -> setText(QApplication::translate("SearchDialog", "Popular", 0));
    gl2 -> addWidget(byPopular, 2, 0);

    byRelativity = new QCheckBox(searchInGroup);
    byRelativity -> setObjectName(QStringLiteral("byRelativity"));
    byRelativity -> setText(QApplication::translate("SearchDialog", "Related", 0));
    gl2 -> addWidget(byRelativity, 0, 1);

    byOrigins = new QCheckBox(searchInGroup);
    byOrigins -> setObjectName(QStringLiteral("byOrigins"));
    byOrigins -> setText(QApplication::translate("SearchDialog", "In originals only", 0));
    gl2 -> addWidget(byOrigins, 1, 1);

    byMixes = new QCheckBox(searchInGroup);
    byMixes -> setObjectName(QStringLiteral("byMixes"));
    byMixes -> setText(QApplication::translate("SearchDialog", "In mixes only", 0));
    gl2 -> addWidget(byMixes, 2, 1);


    byForeign = new QCheckBox(searchInGroup);
    byForeign -> setObjectName(QStringLiteral("byForeign"));
    byForeign -> setText(QApplication::translate("SearchDialog", "In foreign", 0));
    gl2 -> addWidget(byForeign, 0, 2);

    byCyrillic = new QCheckBox(searchInGroup);
    byCyrillic -> setObjectName(QStringLiteral("byCyrillic"));
    byCyrillic -> setText(QApplication::translate("SearchDialog", "In cyrillic", 0));
    gl2 -> addWidget(byCyrillic, 1, 2);


    byOwns = new QCheckBox(searchInGroup);
    byOwns -> setObjectName(QStringLiteral("byOwns"));
    byOwns -> setText(QApplication::translate("SearchDialog", "In owns", 0));
    gl2 -> addWidget(byOwns, 0, 3);


    return limitationsAreaBody;
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
                item -> setData(Qt::UserRole + 1, mdl -> name());
                tabsList -> addItem(item);
            }
        }
    }

    QMap<DataSubType, ISearchable *> sites = Web::Apis::searchersList();
    QListWidgetItem * item = new QListWidgetItem(QString(""));
    item -> setFlags(Qt::NoItemFlags);
    sitesList -> addItem(item);

    for(QMap<DataSubType, ISearchable *>::Iterator it = sites.begin(); it != sites.end(); it++) {
        ISource * src = Web::Apis::source(it.key());

        if (src -> isPermitted(pf_search)) {
            QListWidgetItem * item = new QListWidgetItem(src -> name());
            item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
            item -> setCheckState(Qt::Unchecked);
            item -> setData(Qt::UserRole + 1, it.key());

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

    if (has_not_connected && errors_output)
        errors_output -> setText(QStringLiteral("Some services is not acceptable at this time."));

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

Core::SearchLimitLayers SearchConfigurator::buildParams(
    int items_limit, const SearchSettingsBlocks & blocks, const QStringList & predicates,
    const QStringList & genres, int predicate_types, int content_type)
{   
    SearchLimitLayers res((SearchContentType)content_type, (SearchPredicateType)predicate_types, items_limit);

    for(QStringList::ConstIterator predicate = predicates.cbegin(); predicate != predicates.cend(); predicate++)
        res.predicates.append((*predicate).toLower());

    for(QStringList::ConstIterator genre = genres.cbegin(); genre != genres.cend(); genre++)
        res.genres.append((*genre).toLower());

    if (blocks & block_sites) {
        QList<Core::ISearchable *> searchables = Core::Web::Apis::searchersList().values();
        for(QList<Core::ISearchable *>::Iterator it = searchables.begin(); it != searchables.end(); it++)
            if (Web::Apis::source((*it) -> siteType()) -> isConnected())
                res.sites.append((*it) -> siteType());
    }

    if (blocks & block_tabs) {
        QList<Controls::DockBar *> dockbars = Presentation::Dockbars::obj().dockbars();
        for(QList<Controls::DockBar *>::Iterator it = dockbars.begin(); it != dockbars.end(); it++) {
            Views::IView * v = Presentation::Dockbars::obj().view(*it);
            if (v && !v -> isCommon()) res.tabs.append(((Models::IModel *)v -> model()) -> name());
        }
    }

    if (blocks & block_computer) {
        QFileInfoList drives = QDir::drives();
        for(QFileInfoList::Iterator it = drives.begin(); it != drives.end(); it++)
            res.drives.append((*it).absolutePath());
    }

    return res;
}

SearchConfigurator::SearchConfigurator(QWidget * parent, QPushButton * activationBtn, QLabel * errors_output) : Accordion(parent), activationBtn(activationBtn), has_not_connected(false), errors_output(errors_output) {
    setObjectName(QStringLiteral("searchConfigurator"));
    setExclusive(true);
    setToggleable(false);
    addItem(QStringLiteral("In locations"), initLocations(), true);
    addItem(QStringLiteral("By predicates"), initPredicates());
    addItem(QStringLiteral("With limitations"), initLimitations());

    initiateSources();

    QMetaObject::connectSlotsByName(this);
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(onFocusChanged(QWidget*,QWidget*)));
}

Core::SearchLimitLayers SearchConfigurator::params() {
    int predicate_types = inSites -> isChecked() &&
        textPredicates -> count() == 0 && stylePredicates -> count() == 0 ? Core::sp_popular : Core::sp_title;

    if (byArtist -> isChecked())
        predicate_types = Core::sp_artist;
    else if (bySongName -> isChecked())
        predicate_types = Core::sp_song_name;
    else if (bySet -> isChecked())
        predicate_types = Core::sp_sets;
    else if (byLyric -> isChecked())
        predicate_types = Core::sp_lyrics;
    else if (byAbc -> isChecked())
        predicate_types = Core::sp_abc;
    else if (byTag -> isChecked())
        predicate_types = Core::sp_tag;

    if (byFresh -> isChecked()) predicate_types |= Core::sp_fresh;
    if (byNew -> isChecked()) predicate_types |= Core::sp_new;
    if (byPopular -> isChecked()) predicate_types |= Core::sp_popular;
    if (byRelativity -> isChecked()) predicate_types |= Core::sp_relative;

    if (byCyrillic -> isChecked()) predicate_types |= Core::sp_cyrillic;
    if (byForeign -> isChecked()) predicate_types |= Core::sp_foreign;

    if (byOrigins -> isChecked()) predicate_types |= Core::sp_originals;
    if (byMixes -> isChecked()) predicate_types |= Core::sp_mixes;

    if (byOwns -> isChecked()) predicate_types |= Core::sp_owns;


    SearchContentType content_types = sc_all;

    if (byAudioTypes -> isChecked())
        content_types = Core::sc_audio;
    else if (byVideoTypes -> isChecked())
        content_types = Core::sc_video;


    SearchLimitLayers res(content_types, (SearchPredicateType)predicate_types);

    int count = textPredicates -> count();
    for(int i = 0; i < count; i++)
        res.predicates.append(textPredicates -> item(i) -> text().toLower());

    count = stylePredicates -> count();
    for(int i = 0; i < count; i++)
        res.genres.append(stylePredicates -> item(i) -> text().toLower());

    if (inSites -> isChecked()) {
        int count = sitesList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = sitesList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.sites.append(item -> data(Qt::UserRole + 1).toInt());
        }
    }

    if (inTabs -> isChecked()) {
        int count = tabsList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = tabsList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.tabs.append(item -> data(Qt::UserRole + 1).toString());
        }
    }

    if (inComputer -> isChecked()) {
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

void SearchConfigurator::onFocusChanged(QWidget * /*old*/, QWidget * now) {
    if (now == stylePredicate) {
        activationBtn -> setDefault(false);
        addPredicate -> setDefault(false);
        addStylePredicate -> setDefault(true);
    } else if (now == textPredicate) {
        activationBtn -> setDefault(false);
        addStylePredicate -> setDefault(false);
        addPredicate -> setDefault(true);
    } else {
        addStylePredicate -> setDefault(false);
        addPredicate -> setDefault(false);
        activationBtn -> setDefault(true);
    }
}
