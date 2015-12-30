#include "basic_playlist.h"

using namespace Controls::Echonest;

BasicPlaylist::BasicPlaylist(const QStringList & genresList, QWidget * parent) : QWidget(parent), genresList(genresList) {
    setObjectName(QStringLiteral("BasicPlaylist"));
    generateLayout();
}

void BasicPlaylist::onBasicPlaylistGenerateClicked() {
    ((Controls::DockBar *)parentWidget()) -> onMoveInBackgroundProcess();
    collapseAll();

    QJsonArray results;

    if (artistTypeCheck -> isChecked()) {
        QStringList artists;

        for(QList<QLineEdit *>::Iterator artist = basicPlaylistArtists.begin(); artist != basicPlaylistArtists.end(); artist++) {
            if (!(*artist) -> text().isEmpty())
                artists << (*artist) -> text();
        }

        results = Core::Web::Echonest::Api::obj().playlistBasicByArtists(artists);
    } else if (genreTypeCheck -> isChecked()) {
        QStringList genres;

        for(QList<QComboBox *>::Iterator genre = basicPlaylistGenres.begin(); genre != basicPlaylistGenres.end(); genre++) {
            if (!(*genre) -> currentText().isEmpty())
                genres << (*genre) -> currentText();
        }

        results = Core::Web::Echonest::Api::obj().playlistBasicByGenres(genres);
    }
    else return; // nothing choosed

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



      QStringList predicates;

      for(QJsonArray::Iterator song = results.begin(); song != results.end(); song++) {
          QJsonObject obj = (*song).toObject();
          predicates << (obj.value(QStringLiteral("artist_name")).toString() % QStringLiteral(" - ") % obj.value(QStringLiteral("title")).toString());
//          new WebItem(
//              obj.value("id").toString(),
//              obj.value("artist_id").toString(),
//              obj.value("artist_name").toString() + " - " + obj.value("title").toString(),
//              model -> root()
//          );
      }

      emit playlistGenerationNeed(QStringLiteral("Echonest basic playlist"), predicates);
//      createSearchResultBar(predicates);
//      ((Controls::DockBar *)parentWidget()) -> onMoveOutBackgroundProcess();
//      ((Controls::DockBar *)parentWidget()) -> close();
}

void BasicPlaylist::generateLayout() {
    QWidget * basicPlaylistWidget = new QWidget(this);
    QVBoxLayout * basicPlaylistLayout = new QVBoxLayout(this);

    QGroupBox * playlist_type = new QGroupBox(QStringLiteral("Playlist Type"), basicPlaylistWidget);
    QHBoxLayout * playlist_type_layout = new QHBoxLayout(playlist_type);
    artistTypeCheck = new QRadioButton(QStringLiteral("By similar artists"), playlist_type);
    playlist_type_layout -> addWidget(artistTypeCheck);
    genreTypeCheck = new QRadioButton(QStringLiteral("By genres"), playlist_type);
    playlist_type_layout -> addWidget(genreTypeCheck);

    basicPlaylistLayout -> addWidget(playlist_type);

    QGroupBox * artist_type_fields = new QGroupBox(QStringLiteral("Artists for query"), basicPlaylistWidget);
    QVBoxLayout * artists_layout = new QVBoxLayout(artist_type_fields);
    for(int i = 0; i < 5; i++) {
        QLineEdit * edit = new QLineEdit(artist_type_fields);
        basicPlaylistArtists << edit;
        artists_layout -> addWidget(edit);
    }

    basicPlaylistLayout -> addWidget(artist_type_fields);
    artist_type_fields -> hide();

    QGroupBox * genre_type_fields = new QGroupBox(QStringLiteral("Genres for query"), basicPlaylistWidget);
    QStringList genres = genresList();
    QVBoxLayout * genres_layout = new QVBoxLayout(genre_type_fields);
    for(int i = 0; i < 5; i++) {
        QComboBox * combo = new QComboBox(genre_type_fields);
        combo -> addItems(genres);
        basicPlaylistGenres << combo;
        genres_layout -> addWidget(combo);
    }

    basicPlaylistLayout -> addWidget(genre_type_fields);
    genre_type_fields -> hide();

    connect(artistTypeCheck, SIGNAL(toggled(bool)), artist_type_fields, SLOT(setVisible(bool)));
    connect(genreTypeCheck, SIGNAL(toggled(bool)), genre_type_fields, SLOT(setVisible(bool)));

    QPushButton * basicPlaylistStart = new QPushButton(QStringLiteral("Generate basic playlist"), base);
    connect(basicPlaylistStart, SIGNAL(clicked()), this, SLOT(onBasicPlaylistGenerateClicked()));
    basicPlaylistLayout -> addWidget(basicPlaylistStart);
    setLayout(basicPlaylistLayout);
}
