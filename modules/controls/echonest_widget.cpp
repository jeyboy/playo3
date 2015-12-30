#include "echonest_widget.h"
#include "dockbars.h"

#include "modules/controls/search_configurator.h"
//#include "modules/controls/qxtspanslider.h"

using namespace Controls;

EchonestWidget::EchonestWidget(QWidget * parent) : Controls::Accordion(parent) {
    setObjectName(QStringLiteral("EchonestWidget"));

//    QxtSpanSlider * slider = new QxtSpanSlider(Qt::Horizontal, this);
//    slider -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
//    layout() -> addWidget(slider);

    addItem(QStringLiteral("Artist Info"), new Echonest::ArtistInfo(this));
    Echonest::BasicPlaylist * basicPlaylist = new Echonest::BasicPlaylist(genresList(), this);
    connect(basicPlaylist, SIGNAL(moveInProcess()), this, SLOT(moveInProcess()));
    connect(basicPlaylist, SIGNAL(playlistGenerationNeed(QString,QStringList&)), this, SLOT(playlistGenerationNeed(QString,QStringList&)));
    addItem(QStringLiteral("Base Playlist"), basicPlaylist);
}

void EchonestWidget::moveInProcess() {
    ((Controls::DockBar *)parentWidget()) -> onMoveInBackgroundProcess();
    collapseAll();
}

void EchonestWidget::playlistGenerationNeed(const QString & title, QStringList & predicates) {
    Views::Params settings(Data::search, QString());
    SearchSettings prms = Controls::SearchConfigurator::buildParams(1, Controls::SearchConfigurator::block_tabs_and_sites, predicates);
    Presentation::Dockbars::obj().createDocBar(title, settings, 0, true, true, &prms);

    ((Controls::DockBar *)parentWidget()) -> onMoveOutBackgroundProcess();
    ((Controls::DockBar *)parentWidget()) -> close();
}

//void EchonestWidget::onArtistInfoButtonClicked() {
//    if (!artistName -> text().isEmpty()) {
//        QJsonObject info = Core::Web::Echonest::Api::obj().artistProfile(artistName -> text());

//        artistAccordion -> clear();

//        if (!info.isEmpty()) {
//            QJsonArray biographies = info.value(QStringLiteral("biographies")).toArray();
//            QJsonArray::Iterator biographie = biographies.begin();

//            for(int i = 1; biographie != biographies.end(); biographie++, i++) {
//                QLabel * biographie_label = new QLabel((*biographie).toObject().value(QStringLiteral("text")).toString(), artistAccordion);
//                biographie_label -> setWordWrap(true);
//                artistAccordion -> addItem(QStringLiteral("Biographie ") % QString::number(i), biographie_label);
//            }


//            QWidget * statistic = new QWidget(artistAccordion);
//            QVBoxLayout * l = new QVBoxLayout(statistic);
//            l -> addWidget(new QLabel(
//                QStringLiteral("Familiarity: ") % QString::number((info.value(QStringLiteral("familiarity")).toDouble() * 100)) % QStringLiteral("%"),
//                statistic)
//            );

//            l -> addWidget(new QLabel(
//                QStringLiteral("Hotttnesss: ") % QString::number((info.value(QStringLiteral("hotttnesss")).toDouble() * 100)) % QStringLiteral("%"),
//                statistic)
//            );

//            l -> addWidget(new QLabel(
//                QStringLiteral("Start career from: ") % QString::number(info.value(QStringLiteral("years_active")).toArray().first().toObject().value(QStringLiteral("start")).toInt()),
//                statistic)
//            );


//            QJsonArray genres = info.value(QStringLiteral("genres")).toArray();

//            if (!genres.isEmpty()) {
//                QStringList genresList;

//                for(QJsonArray::Iterator genre = genres.begin(); genre != genres.end(); genre++)
//                    genresList << (*genre).toObject().value(QStringLiteral("name")).toString();

//                l -> addWidget(new QLabel(QStringLiteral("Genres: \n\t") % genresList.join(QStringLiteral("\n\t")), statistic));
//            }

//            QJsonArray terms = info.value(QStringLiteral("terms")).toArray();

//            if (!terms.isEmpty()) {
//                QStringList termsList;

//                for(QJsonArray::Iterator term = terms.begin(); term != terms.end(); term++) {
//                    QJsonObject obj = (*term).toObject();
//                    termsList <<
//                        (
//                            obj.value(QStringLiteral("name")).toString() % QStringLiteral("\n") %
//                                QStringLiteral("\t\tfrequency: ") % QString::number(obj.value(QStringLiteral("frequency")).toDouble() * 100) % QStringLiteral("%\n") %
//                                QStringLiteral("\t\tweight: ") % QString::number(obj.value(QStringLiteral("weight")).toDouble() * 100) % QStringLiteral("%")
//                        );
//                }

//                QLabel * termsLabel = new QLabel(QStringLiteral("Terms: \n\t") % termsList.join(QStringLiteral("\n\t")), statistic);
//                termsLabel -> setWordWrap(true);
//                l -> addWidget(termsLabel);
//            }

//            artistAccordion -> addItem(QStringLiteral("Statistic"), statistic);



//            QJsonArray news = info.value(QStringLiteral("news")).toArray();

//            if (!news.isEmpty()) {
//                QWidget * newsBlock = new QWidget(artistAccordion);
//                QVBoxLayout * il = new QVBoxLayout(newsBlock);

//                for(QJsonArray::Iterator news_item = news.begin(); news_item != news.end(); news_item++) {
//                    QJsonObject obj = (*news_item).toObject();
//                    QLabel * newsLabel = new QLabel(
//                                obj.value(QStringLiteral("date_found")).toString() % QStringLiteral("\n") % obj.value(QStringLiteral("name")).toString() % QStringLiteral("\n\n") % obj.value(QStringLiteral("summary")).toString()
//                                , newsBlock);
//                    newsLabel -> setWordWrap(true);
//                    il -> addWidget(newsLabel);
//                }

//                artistAccordion -> addItem(QStringLiteral("News"), newsBlock);
//            }

//            QJsonArray songs = info.value(QStringLiteral("songs")).toArray();

//            if (!songs.isEmpty()) {
//                QWidget * songsBlock = new QWidget(artistAccordion);
//                QVBoxLayout * il = new QVBoxLayout(songsBlock);
//                QHash<QString, bool> songsHash;

//                for(QJsonArray::Iterator song = songs.begin(); song != songs.end(); song++) {
//                    QJsonObject obj = (*song).toObject();

//                    QString str = obj.value(QStringLiteral("title")).toString();

//                    if (!songsHash.contains(str)) {
//                        QLabel * newsLabel = new QLabel(str, songsBlock);
//    //                    obj.value("name").toString()
//                        il -> addWidget(newsLabel);
//                        songsHash.insert(str, true);
//                    }
//                }

//                artistAccordion -> addItem(QStringLiteral("Songs"), songsBlock);
//            }

//            QJsonArray images = info.value(QStringLiteral("images")).toArray();

//            if (!images.isEmpty()) {
//                QWidget * imagesBlock = new QWidget(artistAccordion);
//                QVBoxLayout * il = new QVBoxLayout(imagesBlock);

//                for(QJsonArray::Iterator image = images.begin(); image != images.end(); image++) {
//                    QLabel * imageLabel = new QLabel(imagesBlock);
//                    QUrl url((*image).toObject().value(QStringLiteral("url")).toString());
//                    QPixmap pixma = Core::Web::Manager::prepare() -> getImage(url);
//                    pixma = pixma.scaledToWidth(artistAccordion -> width() - 60, Qt::SmoothTransformation);

//                    imageLabel -> setPixmap(pixma);
//                    il -> addWidget(imageLabel);
//                }

//                artistAccordion -> addItem(QStringLiteral("Images"), imagesBlock);
//            }
//        }
//    }
//}

//void EchonestWidget::onBasicPlaylistGenerateClicked() {
//    ((Controls::DockBar *)parentWidget()) -> onMoveInBackgroundProcess();
//    collapseAll();

//    QJsonArray results;

//    if (artistTypeCheck -> isChecked()) {
//        QStringList artists;

//        for(QList<QLineEdit *>::Iterator artist = basicPlaylistArtists.begin(); artist != basicPlaylistArtists.end(); artist++) {
//            if (!(*artist) -> text().isEmpty())
//                artists << (*artist) -> text();
//        }

//        results = Core::Web::Echonest::Api::obj().playlistBasicByArtists(artists);
//    } else if (genreTypeCheck -> isChecked()) {
//        QStringList genres;

//        for(QList<QComboBox *>::Iterator genre = basicPlaylistGenres.begin(); genre != basicPlaylistGenres.end(); genre++) {
//            if (!(*genre) -> currentText().isEmpty())
//                genres << (*genre) -> currentText();
//        }

//        results = Core::Web::Echonest::Api::obj().playlistBasicByGenres(genres);
//    }
//    else return; // nothing choosed

//    //            {
//    //                "artist_foreign_ids": [
//    //                    {
//    //                        "catalog": "7digital-US",
//    //                        "foreign_id": "7digital-US:artist:7516"
//    //                    }
//    //                ],
//    //                "artist_id": "AR633SY1187B9AC3B9",
//    //                "artist_name": "Weezer",
//    //                "id": "SOBSLVH12A8C131F38",
//    //                "title": "Island In The Sun",
//    //                "tracks": [
//    //                    {
//    //                        "catalog": "7digital-US",
//    //                        "foreign_id": "7digital-US:track:20637990",
//    //                        "foreign_release_id": "7digital-US:release:1914387",
//    //                        "id": "TRTXLYU13A79B0B112",
//    //                        "preview_url": "http://previews.7digital.com/clips/34/20637990.clip.mp3",
//    //                        "release_image": "http://cdn.7static.com/static/img/sleeveart/00/019/143/0001914387_200.jpg"
//    //                    }
//    //                ]
//    //            }



//      QStringList predicates;

//      for(QJsonArray::Iterator song = results.begin(); song != results.end(); song++) {
//          QJsonObject obj = (*song).toObject();
//          predicates << (obj.value(QStringLiteral("artist_name")).toString() % QStringLiteral(" - ") % obj.value(QStringLiteral("title")).toString());
////          new WebItem(
////              obj.value("id").toString(),
////              obj.value("artist_id").toString(),
////              obj.value("artist_name").toString() + " - " + obj.value("title").toString(),
////              model -> root()
////          );
//      }

//      createSearchResultBar(predicates);
//      ((Controls::DockBar *)parentWidget()) -> onMoveOutBackgroundProcess();
//      ((Controls::DockBar *)parentWidget()) -> close();
//}

//void EchonestWidget::artistInfoGeneration(QWidget * base) {
//    QGridLayout * artistInfoLayout = new QGridLayout();

//    artistName = new QLineEdit(base);
//    artistInfoLayout -> addWidget(artistName, 0, 0);

//    connect(artistName, SIGNAL(returnPressed()), this, SLOT(onArtistInfoButtonClicked()));

//    QPushButton * artistInfoButton = new QPushButton(QStringLiteral("Find Info"), base);
//    connect(artistInfoButton, SIGNAL(clicked()), this, SLOT(onArtistInfoButtonClicked()));
//    artistInfoLayout -> addWidget(artistInfoButton, 0, 1);

//    artistAccordion = new Controls::Accordion(base);
//    artistInfoLayout -> addWidget(artistAccordion, 1, 0, 10, 2);

//    base -> setLayout(artistInfoLayout);
//}

//void EchonestWidget::songsSearch(QWidget * base) {
//    QWidget * songSearchWidget = new QWidget(base);
//    QVBoxLayout * songsSearchLayout = new QVBoxLayout(songSearchWidget);
//    QGroupBox * playlist_type = new QGroupBox(QStringLiteral("Search by artist or/and title"), songSearchWidget);

//    artistName = new QLineEdit(base);
//    artistInfoLayout -> addWidget(artistName, 0, 0);
//}

//void EchonestWidget::basicPlaylistGeneration(QWidget * base) {
//    QWidget * basicPlaylistWidget = new QWidget(base);
//    QVBoxLayout * basicPlaylistLayout = new QVBoxLayout(base);

//    QGroupBox * playlist_type = new QGroupBox(QStringLiteral("Playlist Type"), basicPlaylistWidget);
//    QHBoxLayout * playlist_type_layout = new QHBoxLayout(playlist_type);
//    artistTypeCheck = new QRadioButton(QStringLiteral("By similar artists"), playlist_type);
//    playlist_type_layout -> addWidget(artistTypeCheck);
//    genreTypeCheck = new QRadioButton(QStringLiteral("By genres"), playlist_type);
//    playlist_type_layout -> addWidget(genreTypeCheck);

//    basicPlaylistLayout -> addWidget(playlist_type);

//    QGroupBox * artist_type_fields = new QGroupBox(QStringLiteral("Artists for query"), basicPlaylistWidget);
//    QVBoxLayout * artists_layout = new QVBoxLayout(artist_type_fields);
//    for(int i = 0; i < 5; i++) {
//        QLineEdit * edit = new QLineEdit(artist_type_fields);
//        basicPlaylistArtists << edit;
//        artists_layout -> addWidget(edit);
//    }

//    basicPlaylistLayout -> addWidget(artist_type_fields);
//    artist_type_fields -> hide();

//    QGroupBox * genre_type_fields = new QGroupBox(QStringLiteral("Genres for query"), basicPlaylistWidget);
//    QStringList genres = genresList();
//    QVBoxLayout * genres_layout = new QVBoxLayout(genre_type_fields);
//    for(int i = 0; i < 5; i++) {
//        QComboBox * combo = new QComboBox(genre_type_fields);
//        combo -> addItems(genres);
//        basicPlaylistGenres << combo;
//        genres_layout -> addWidget(combo);
//    }

//    basicPlaylistLayout -> addWidget(genre_type_fields);
//    genre_type_fields -> hide();

//    connect(artistTypeCheck, SIGNAL(toggled(bool)), artist_type_fields, SLOT(setVisible(bool)));
//    connect(genreTypeCheck, SIGNAL(toggled(bool)), genre_type_fields, SLOT(setVisible(bool)));

//    QPushButton * basicPlaylistStart = new QPushButton(QStringLiteral("Generate basic playlist"), base);
//    connect(basicPlaylistStart, SIGNAL(clicked()), this, SLOT(onBasicPlaylistGenerateClicked()));
//    basicPlaylistLayout -> addWidget(basicPlaylistStart);
//}

QStringList EchonestWidget::genresList() {
    if (genres.isEmpty()) {
        genres << QStringLiteral("");
        genres.append(Core::Web::Echonest::Api::obj().genresList().genresList());
    }

    return styles;
}

QStringList EchonestWidget::stylesList() {
    if (styles.isEmpty()) {
        styles << QStringLiteral("");

        QJsonArray styles = Core::Web::Echonest::Api::obj().artistStylesForSearch();
        for(QJsonArray::Iterator style = styles.begin(); style != styles.end(); style++)
            styles.append((*style).toObject().value(QStringLiteral("name")).toString());
    }

    return styles;
}

QStringList EchonestWidget::moodsList() {
    if (moods.isEmpty()) {
        QJsonArray moodsList = Core::Web::Echonest::Api::obj().artistMoodsForSearch();
        for(QJsonArray::Iterator mood = moodsList.begin(); mood != moodsList.end(); mood++)
            moods.append((*mood).toObject().value(QStringLiteral("name")).toString());
    }

    return moods;
}
