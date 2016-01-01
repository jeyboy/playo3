#include "song_search.h"

using namespace Controls::Echonest;

SongSearch::SongSearch(const QStringList & stylesList, const QStringList & moodsList, QWidget * parent) : QWidget(parent), stylesList(stylesList), moodsList(moodsList) {
    setObjectName(QStringLiteral("SongSearch"));
    generateLayout();
}

void SongSearch::onSearchClicked() {
//    emit moveInProcess();

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
//      }

//      emit playlistGenerationNeed(QStringLiteral("Echonest basic playlist"), predicates);
}

void SongSearch::generateLayout() {
    QVBoxLayout * songSearchLayout = new QVBoxLayout(this);

    QGroupBox * songAttrGroup = new QGroupBox(QStringLiteral("Song attrs"), this);
    songSearchLayout -> addWidget(songAttrGroup);


    //    QHBoxLayout * playlist_type_layout = new QHBoxLayout(playlist_type);
    //    artistTypeCheck = new QRadioButton(QStringLiteral("By similar artists"), playlist_type);
    //    playlist_type_layout -> addWidget(artistTypeCheck);
    //    genreTypeCheck = new QRadioButton(QStringLiteral("By genres"), playlist_type);
    //    playlist_type_layout -> addWidget(genreTypeCheck);

    setLayout(songSearchLayout);

    artist = new QLineEdit(songAttrGroup);
    title = new QLineEdit(songAttrGroup);
    combined = new QCheckBox(songAttrGroup);


    QGroupBox * songCredentilsGroup = new QGroupBox(QStringLiteral("Song credentials"), this);
    songSearchLayout -> addWidget(songCredentilsGroup);


    mode = new QComboBox(songCredentilsGroup);
    mode -> addItem(QStringLiteral("All"));
    mode -> addItem(QStringLiteral("Minor"), 0);
    mode -> addItem(QStringLiteral("Major"), 1);

    artist_familiarity = new QxtSpanSlider(Qt::Horizontal, this);
    artist_familiarity -> setMinimum(0); artist_familiarity -> setMaximum(1000);
    artist_familiarity -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(artist_familiarity);

    song_hotttnesss = new QxtSpanSlider(Qt::Horizontal, this);
    song_hotttnesss -> setMinimum(0); song_hotttnesss -> setMaximum(1000);
    song_hotttnesss -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_hotttnesss);

    song_danceability = new QxtSpanSlider(Qt::Horizontal, this);
    song_danceability -> setMinimum(0); song_danceability -> setMaximum(1000);
    song_danceability -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_danceability);

    song_energy = new QxtSpanSlider(Qt::Horizontal, this);
    song_energy -> setMinimum(0); song_energy -> setMaximum(1000);
    song_energy -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_energy);

    song_liveness = new QxtSpanSlider(Qt::Horizontal, this);
    song_liveness -> setMinimum(0); song_liveness -> setMaximum(1000);
    song_liveness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_liveness);

    song_speechiness = new QxtSpanSlider(Qt::Horizontal, this);
    song_speechiness -> setMinimum(0); song_speechiness -> setMaximum(1000);
    song_speechiness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_speechiness);

    song_acousticness = new QxtSpanSlider(Qt::Horizontal, this);
    song_acousticness -> setMinimum(0); song_acousticness -> setMaximum(1000);
    song_acousticness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_acousticness);

    song_tempo = new QxtSpanSlider(Qt::Horizontal, this);
    song_tempo -> setMinimum(0); song_tempo -> setMaximum(1000);
    song_tempo -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songSearchLayout -> addWidget(song_tempo);


    QWidget * songDescriptions = new QWidget(this);
    songSearchLayout -> addWidget(songDescriptions);


//    QList<QLineEdit *> descriptions;
//    QList<QDoubleSpinBox *> descriptionPowers;
//    QList<QComboBox *> styles;
//    QList<QDoubleSpinBox *> stylePowers;
//    QList<QComboBox *> moods;
//    QList<QDoubleSpinBox *> moodPowers;







//    QWidget * basicPlaylistWidget = new QWidget(this);
//    QVBoxLayout * basicPlaylistLayout = new QVBoxLayout(this);

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
//    QVBoxLayout * genres_layout = new QVBoxLayout(genre_type_fields);
//    for(int i = 0; i < 5; i++) {
//        QComboBox * combo = new QComboBox(genre_type_fields);
//        combo -> addItems(genresList);
//        basicPlaylistGenres << combo;
//        genres_layout -> addWidget(combo);
//    }

//    basicPlaylistLayout -> addWidget(genre_type_fields);
//    genre_type_fields -> hide();

//    connect(artistTypeCheck, SIGNAL(toggled(bool)), artist_type_fields, SLOT(setVisible(bool)));
//    connect(genreTypeCheck, SIGNAL(toggled(bool)), genre_type_fields, SLOT(setVisible(bool)));

//    QPushButton * basicPlaylistStart = new QPushButton(QStringLiteral("Generate basic playlist"), this);
//    connect(basicPlaylistStart, SIGNAL(clicked()), this, SLOT(onBasicPlaylistGenerateClicked()));
//    basicPlaylistLayout -> addWidget(basicPlaylistStart);
//    setLayout(basicPlaylistLayout);
}
