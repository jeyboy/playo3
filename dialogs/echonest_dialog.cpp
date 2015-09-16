#include "echonest_dialog.h"
#include "ui_echonest_dialog.h"
#include <qdebug.h>

#include "double_slider/qxtspanslider.h"

EchonestDialog::EchonestDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::EchonestDialog) {
    ui -> setupUi(this);

    Accordion * accordion = new Accordion(this);

    QxtSpanSlider * slider = new QxtSpanSlider(Qt::Horizontal, this);
    slider -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    layout() -> addWidget(slider);

    layout() -> addWidget(accordion);

    artistInfoGeneration(accordion -> addItem(QStringLiteral("Artist Info")));
    basicPlaylistGeneration(accordion -> addItem(QStringLiteral("Base Playlist")));
}

EchonestDialog::~EchonestDialog() {
    delete ui;
}

void EchonestDialog::onArtistInfoButtonClicked() {
    if (!artistName -> text().isEmpty()) {
        Json::Obj info = EchonestApi::instance() -> artistProfile(artistName -> text());

        artistAccordion -> clear();

        if (!info.isEmpty()) {
            Json::Arr biographies = info.arr(QStringLiteral("biographies"));
            Json::Arr::Iterator biographie = biographies.begin();

            for(int i = 1; biographie != biographies.end(); biographie++, i++) {
                QLabel * biographie_label = new QLabel((*biographie).toObject().value(QStringLiteral("text")).toString(), artistAccordion);
                biographie_label -> setWordWrap(true);
                artistAccordion -> addItem(QStringLiteral("Biographie ") % QString::number(i), biographie_label);
            }


            QWidget * statistic = new QWidget(artistAccordion);
            QVBoxLayout * l = new QVBoxLayout(statistic);
            l -> addWidget(new QLabel(
                QStringLiteral("Familiarity: ") % QString::number((info.value(QStringLiteral("familiarity")).toDouble() * 100)) % QStringLiteral("%"),
                statistic)
            );

            l -> addWidget(new QLabel(
                QStringLiteral("Hotttnesss: ") % QString::number((info.value(QStringLiteral("hotttnesss")).toDouble() * 100)) % QStringLiteral("%"),
                statistic)
            );

            l -> addWidget(new QLabel(
                QStringLiteral("Start career from: ") % QString::number(info.value(QStringLiteral("years_active")).toArray().first().toObject().value(QStringLiteral("start")).toInt()),
                statistic)
            );


            Json::Arr genres = info.arr(QStringLiteral("genres"));

            if (!genres.isEmpty()) {
                QStringList genresList;

                for(Json::Arr::Iterator genre = genres.begin(); genre != genres.end(); genre++)
                    genresList << (*genre).toObject().value(QStringLiteral("name")).toString();

                l -> addWidget(new QLabel(QStringLiteral("Genres: \n\t") % genresList.join(QStringLiteral("\n\t")), statistic));
            }

            Json::Arr terms = info.arr(QStringLiteral("terms"));

            if (!terms.isEmpty()) {
                QStringList termsList;

                for(Json::Arr::Iterator term = terms.begin(); term != terms.end(); term++) {
                    Json::Obj obj = Json::Val::fromQVal(*term).obj();
                    termsList <<
                        (
                            obj.value(QStringLiteral("name")).toString() % QStringLiteral("\n") %
                                QStringLiteral("\t\tfrequency: ") % QString::number(obj.value(QStringLiteral("frequency")).toDouble() * 100) % QStringLiteral("%\n") %
                                QStringLiteral("\t\tweight: ") % QString::number(obj.value(QStringLiteral("weight")).toDouble() * 100) % QStringLiteral("%")
                        );
                }

                QLabel * termsLabel = new QLabel(QStringLiteral("Terms: \n\t") % termsList.join(QStringLiteral("\n\t")), statistic);
                termsLabel -> setWordWrap(true);
                l -> addWidget(termsLabel);
            }

            artistAccordion -> addItem(QStringLiteral("Statistic"), statistic);


            Json::Arr news = info.arr(QStringLiteral("news"));

            if (!news.isEmpty()) {
                QWidget * newsBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(newsBlock);

                for(Json::Arr::Iterator news_item = news.begin(); news_item != news.end(); news_item++) {
                    Json::Obj obj = Json::Val::fromQVal(*news_item).obj();
                    QLabel * newsLabel = new QLabel(
                                obj.value(QStringLiteral("date_found")).toString() % QStringLiteral("\n") % obj.value(QStringLiteral("name")).toString() % QStringLiteral("\n\n") % obj.value(QStringLiteral("summary")).toString()
                                , newsBlock);
                    newsLabel -> setWordWrap(true);
                    il -> addWidget(newsLabel);
                }

                artistAccordion -> addItem(QStringLiteral("News"), newsBlock);
            }

            Json::Arr songs = info.arr(QStringLiteral("songs"));

            if (!songs.isEmpty()) {
                QWidget * songsBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(songsBlock);
                QHash<QString, bool> songsHash;

                for(Json::Arr::Iterator song = songs.begin(); song != songs.end(); song++) {
                    Json::Obj obj = Json::Val::fromQVal(*song).obj();

                    QString str = obj.str(QStringLiteral("title"));

                    if (!songsHash.contains(str)) {
                        QLabel * newsLabel = new QLabel(str, songsBlock);
    //                    obj.value("name").toString()
                        il -> addWidget(newsLabel);
                        songsHash.insert(str, true);
                    }
                }

                artistAccordion -> addItem(QStringLiteral("Songs"), songsBlock);
            }

            Json::Arr images = info.arr(QStringLiteral("images"));

            if (!images.isEmpty()) {
                QWidget * imagesBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(imagesBlock);

                for(Json::Arr::Iterator image = images.begin(); image != images.end(); image++) {
                    QLabel * imageLabel = new QLabel(imagesBlock);
                    QUrl url((*image).toObject().value(QStringLiteral("url")).toString());
                    QPixmap pixma = WebManager::manager() -> getImage(url);
                    pixma = pixma.scaledToWidth(artistAccordion -> width() - 60, Qt::SmoothTransformation);

                    imageLabel -> setPixmap(pixma);
                    il -> addWidget(imageLabel);
                }

                artistAccordion -> addItem(QStringLiteral("Images"), imagesBlock);
            }
        }
    }
}

void EchonestDialog::onBasicPlaylistGenerateClicked() {
    Json::Arr results;

    if (artistTypeCheck -> isChecked()) {
        QStringList artists;

        for(QList<QLineEdit *>::Iterator artist = basicPlaylistArtists.begin(); artist != basicPlaylistArtists.end(); artist++) {
            if (!(*artist) -> text().isEmpty())
                artists << (*artist) -> text();
        }

        results = EchonestApi::instance() -> playlistBasicByArtists(artists);
    } else if (genreTypeCheck -> isChecked()) {
        QStringList genres;

        for(QList<QComboBox *>::Iterator genre = basicPlaylistGenres.begin(); genre != basicPlaylistGenres.end(); genre++) {
            if (!(*genre) -> currentText().isEmpty())
                genres << (*genre) -> currentText();
        }

        results = EchonestApi::instance() -> playlistBasicByGenres(genres);
    } else {
        return;
        // nothing choosed
    }

    //            {
    //                "artist_foreign_ids": [
    //                    {
    //                        "catalog": "7digital-US",
    //                        "foreign_id": "7digital-US:artist:7516"
    //                    }
    //                ],
    //                "artist_id": "AR633SY1187B9AC3B9",
    //                "artist_name": "Weezer",
    //                "id": "SOBSLVH12A8C131F38",
    //                "title": "Island In The Sun",
    //                "tracks": [
    //                    {
    //                        "catalog": "7digital-US",
    //                        "foreign_id": "7digital-US:track:20637990",
    //                        "foreign_release_id": "7digital-US:release:1914387",
    //                        "id": "TRTXLYU13A79B0B112",
    //                        "preview_url": "http://previews.7digital.com/clips/34/20637990.clip.mp3",
    //                        "release_image": "http://cdn.7static.com/static/img/sleeveart/00/019/143/0001914387_200.jpg"
    //                    }
    //                ]
    //            }

      SearchView * view = new SearchView(this, ViewSettings::echo());
      QStringList predicates;

      qDebug() << results;

      for(Json::Arr::Iterator song = results.begin(); song != results.end(); song++) {
          Json::Obj obj = Json::Val::fromQVal(*song).obj();
          predicates << (obj.str(QStringLiteral("artist_name")) + " - " + obj.str(QStringLiteral("title")));
//          new WebItem(
//              obj.value("id").toString(),
//              obj.value("artist_id").toString(),
//              obj.value("artist_name").toString() + " - " + obj.value("title").toString(),
//              model -> root()
//          );
      }

      qDebug() << "ELO" << predicates;
      view -> search(predicates);

//    view -> reset();
    ((QWidget *)sender()) -> parentWidget() -> layout() -> addWidget(view);
}

void EchonestDialog::artistInfoGeneration(QWidget * base) {
    QGridLayout * gl = new QGridLayout();

    artistName = new QLineEdit(base);
    gl -> addWidget(artistName, 0, 0);

    connect(artistName, SIGNAL(returnPressed()), this, SLOT(onArtistInfoButtonClicked()));

    QPushButton * artistInfoButton = new QPushButton(QStringLiteral("Find Info"), base);
    connect(artistInfoButton, SIGNAL(clicked()), this, SLOT(onArtistInfoButtonClicked()));
    gl -> addWidget(artistInfoButton, 0, 1);

    artistAccordion = new Accordion(base);
    gl -> addWidget(artistAccordion, 1, 0, 10, 2);

    base -> setLayout(gl);
}

void EchonestDialog::basicPlaylistGeneration(QWidget * base) {
    QVBoxLayout * layoutSimilar = new QVBoxLayout(base);

    QWidget * playlistType = new QWidget(base);

    QGroupBox * playlist_type = new QGroupBox(QStringLiteral("Playlist Type"), playlistType);
    QHBoxLayout * playlist_type_layout = new QHBoxLayout(playlist_type);
    artistTypeCheck = new QRadioButton(QStringLiteral("By similar artists"), playlist_type);
    playlist_type_layout -> addWidget(artistTypeCheck);
    genreTypeCheck = new QRadioButton(QStringLiteral("By genres"), playlist_type);
    playlist_type_layout -> addWidget(genreTypeCheck);

    layoutSimilar -> addWidget(playlist_type);

    QGroupBox * artist_type_fields = new QGroupBox(QStringLiteral("Artists for query"), playlistType);
    QVBoxLayout * artists_layout = new QVBoxLayout(artist_type_fields);
    for(int i = 0; i < 5; i++) {
        QLineEdit * edit = new QLineEdit(artist_type_fields);
        basicPlaylistArtists << edit;
        artists_layout -> addWidget(edit);
    }

    layoutSimilar -> addWidget(artist_type_fields);
    artist_type_fields -> hide();

    QGroupBox * genre_type_fields = new QGroupBox(QStringLiteral("Genres for query"), playlistType);
    QStringList genres = genresList();
    QVBoxLayout * genres_layout = new QVBoxLayout(genre_type_fields);
    for(int i = 0; i < 5; i++) {
        QComboBox * combo = new QComboBox(genre_type_fields);
        combo -> addItems(genres);
        basicPlaylistGenres << combo;
        genres_layout -> addWidget(combo);
    }

    layoutSimilar -> addWidget(genre_type_fields);
    genre_type_fields -> hide();

    connect(artistTypeCheck, SIGNAL(toggled(bool)), artist_type_fields, SLOT(setVisible(bool)));
    connect(genreTypeCheck, SIGNAL(toggled(bool)), genre_type_fields, SLOT(setVisible(bool)));

    QPushButton * basicPlaylistStart = new QPushButton(QStringLiteral("Generate basic playlist"), base);
    connect(basicPlaylistStart, SIGNAL(clicked()), this, SLOT(onBasicPlaylistGenerateClicked()));
    layoutSimilar -> addWidget(basicPlaylistStart);
}

QStringList EchonestDialog::genresList() {
    QStringList styles; styles << QStringLiteral("");
    styles.append(EchonestApi::instance() -> genresList().genresList());
    return styles;
}

QStringList EchonestDialog::stylesList() {
    QStringList styles; styles << QStringLiteral("");

    Json::Arr genres = EchonestApi::instance() -> artistStylesForSearch();
    for(Json::Arr::Iterator genre = genres.begin(); genre != genres.end(); genre++)
        styles.append((*genre).toObject().value(QStringLiteral("name")).toString());

    return styles;
}

QStringList EchonestDialog::moodsList() {
    QStringList moodsList;

    Json::Arr moods = EchonestApi::instance() -> artistMoodsForSearch();
    for(Json::Arr::Iterator mood = moods.begin(); mood != moods.end(); mood++)
        moodsList.append((*mood).toObject().value(QStringLiteral("name")).toString());

    return moodsList;
}
