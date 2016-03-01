#include "echonest_widget.h"
#include "dockbars.h"

#include "modules/controls/search_configurator.h"

using namespace Controls;

EchonestWidget::EchonestWidget(QWidget * parent) : Controls::Accordion(parent) {
    setObjectName(QStringLiteral("EchonestWidget"));

    addItem(QStringLiteral("Artist Info"), new Echonest::ArtistInfo(this));

    Echonest::BasicPlaylist * basicPlaylist = new Echonest::BasicPlaylist(genresList(), this);
    connect(basicPlaylist, SIGNAL(moveInProcess()), this, SLOT(moveInProcess()));
    connect(basicPlaylist, SIGNAL(playlistGenerationNeed(QString,QStringList&)), this, SLOT(playlistGenerationNeed(QString,QStringList&)));
    addItem(QStringLiteral("Base Playlist"), basicPlaylist);

    Echonest::SongSearch * songSearch = new Echonest::SongSearch(stylesList(), moodsList(), this);
    connect(songSearch, SIGNAL(moveInProcess()), this, SLOT(moveInProcess()));
    connect(songSearch, SIGNAL(playlistGenerationNeed(QString,QStringList&)), this, SLOT(playlistGenerationNeed(QString,QStringList&)));
    addItem(QStringLiteral("Songs Search"), songSearch);
}

void EchonestWidget::moveInProcess() {
    ((Controls::DockBar *)parentWidget()) -> onMoveInBackgroundProcess();
    collapseAll();
}

void EchonestWidget::playlistGenerationNeed(const QString & title, QStringList & predicates) {
    Views::Params settings(Core::playlist_search, QString());
    SearchSettings prms = Controls::SearchConfigurator::buildParams(1, Controls::SearchConfigurator::block_tabs_and_sites, predicates);
    Presentation::Dockbars::obj().createDocBar(title, settings, 0, true, true, &prms);

    ((Controls::DockBar *)parentWidget()) -> onMoveOutBackgroundProcess();
    ((Controls::DockBar *)parentWidget()) -> close();
}

QStringList EchonestWidget::genresList() {
    if (genres.isEmpty()) {
        genres.append(Core::Web::Echonest::Api::obj().genresList().genresList());
    }

    return styles;
}

QStringList EchonestWidget::stylesList() {
    if (styles.isEmpty()) {
        QJsonArray stylesObjs = Core::Web::Echonest::Api::obj().artistStylesForSearch();
        for(QJsonArray::Iterator style = stylesObjs.begin(); style != stylesObjs.end(); style++) {
            styles.append((*style).toObject().value(QStringLiteral("name")).toString());
        }
    }

    return styles;
}

QStringList EchonestWidget::moodsList() {
    if (moods.isEmpty()) {
        QJsonArray moodsObjs = Core::Web::Echonest::Api::obj().artistMoodsForSearch();
        for(QJsonArray::Iterator mood = moodsObjs.begin(); mood != moodsObjs.end(); mood++)
            moods.append((*mood).toObject().value(QStringLiteral("name")).toString());
    }

    return moods;
}
