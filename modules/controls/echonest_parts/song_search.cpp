#include "song_search.h"

using namespace Controls::Echonest;

SongSearch::SongSearch(const QStringList & stylesList, const QStringList & moodsList, QWidget * parent) : QWidget(parent), stylesList(stylesList), moodsList(moodsList) {
    setObjectName(QStringLiteral("SongSearch"));
    generateLayout();
}

void SongSearch::onDescriptionAdd() {
    QWidget * descBlock = new QWidget(descsLayout -> widget());
    descsLayout -> addWidget(descBlock);

    QHBoxLayout * descBlockLayout = new QHBoxLayout(descBlock);

    QLineEdit * edit = new QLineEdit(descBlock);
    descBlockLayout -> addWidget(edit);
    edit -> setProperty("parent", (int)descBlock);

    QDoubleSpinBox * power = new QDoubleSpinBox(descBlock);
    power -> setValue(1);
    descBlockLayout -> addWidget(edit);

    descriptions.insert(edit, power);

    QPushButton * remove = new QPushButton(descBlock);
    remove -> setProperty("parent", (int)edit);
    descBlockLayout -> addWidget(edit);
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(onDescriptionRemove()));
}
void SongSearch::onDescriptionRemove() {
    QPushButton * remove = (QPushButton *)sender();
    QLineEdit * edit = (QLineEdit *)remove -> property("parent").toInt();
    QWidget * descBlock = (QWidget *) edit -> property("parent").toInt();
    descriptions.remove(edit);
    descBlock -> deleteLater();
}

void SongSearch::onStyleAdd() {

}
void SongSearch::onStyleRemove() {

}

void SongSearch::onMoodAdd() {

}
void SongSearch::onMoodRemove() {

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
    QVBoxLayout * songAttrsLayout = new QVBoxLayout(songAttrGroup);
    songSearchLayout -> addWidget(songAttrGroup);

    songAttrsLayout -> addWidget((combined = new QCheckBox(QStringLiteral("Search by artist and title"), songAttrGroup)));
    songAttrsLayout -> addWidget(new QLabel(QStringLiteral("Artist"), songAttrGroup));
    songAttrsLayout -> addWidget((artist = new QLineEdit(songAttrGroup)));
    songAttrsLayout -> addWidget(new QLabel(QStringLiteral("Song title"), songAttrGroup));
    songAttrsLayout -> addWidget((title = new QLineEdit(songAttrGroup)));


    QGroupBox * songCredentilsGroup = new QGroupBox(QStringLiteral("Song credentials"), this);
    QVBoxLayout * songCredentilsLayout = new QVBoxLayout(songCredentilsGroup);
    songSearchLayout -> addWidget(songCredentilsGroup);


    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Tonality of songs"), songCredentilsGroup));
    mode = new QComboBox(songCredentilsGroup);
    mode -> addItem(QStringLiteral("All"));
    mode -> addItem(QStringLiteral("Minor"), 0);
    mode -> addItem(QStringLiteral("Major"), 1);
    songCredentilsLayout -> addWidget(mode);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Artist familiarity"), songCredentilsGroup));
    artist_familiarity = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    artist_familiarity -> setMinimum(0); artist_familiarity -> setMaximum(1000);
    artist_familiarity -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(artist_familiarity);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song hotttnesss"), songCredentilsGroup));
    song_hotttnesss = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_hotttnesss -> setMinimum(0); song_hotttnesss -> setMaximum(1000);
    song_hotttnesss -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_hotttnesss);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song daneability"), songCredentilsGroup));
    song_danceability = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_danceability -> setMinimum(0); song_danceability -> setMaximum(1000);
    song_danceability -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_danceability);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song energy"), songCredentilsGroup));
    song_energy = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_energy -> setMinimum(0); song_energy -> setMaximum(1000);
    song_energy -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_energy);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song liveness"), songCredentilsGroup));
    song_liveness = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_liveness -> setMinimum(0); song_liveness -> setMaximum(1000);
    song_liveness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_liveness);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song speechiness"), songCredentilsGroup));
    song_speechiness = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_speechiness -> setMinimum(0); song_speechiness -> setMaximum(1000);
    song_speechiness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_speechiness);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song acousticness"), songCredentilsGroup));
    song_acousticness = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_acousticness -> setMinimum(0); song_acousticness -> setMaximum(1000);
    song_acousticness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_acousticness);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Song tempo"), songCredentilsGroup));
    song_tempo = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    song_tempo -> setMinimum(0); song_tempo -> setMaximum(1000);
    song_tempo -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_tempo);



    QGroupBox * songChracterizationGroup = new QGroupBox(QStringLiteral("Song characrezition"), this);
    QVBoxLayout * songChracterizationLayout = new QVBoxLayout(songChracterizationGroup);
    songSearchLayout -> addWidget(songChracterizationGroup);


    // descriptions block
    QWidget * songDescriptions = new QWidget(songChracterizationGroup);
    songChracterizationLayout -> addWidget(songDescriptions);

    descsLayout = new QVBoxLayout(songDescriptions);
    songDescriptions -> setLayout(descsLayout);

    QPushButton * addDescription = new QPushButton(QStringLiteral("Add Description creteria"), songChracterizationGroup);
    songChracterizationLayout -> addWidget(addDescription);
    connect(addDescription, SIGNAL(clicked(bool)), this, SLOT(onDescriptionAdd()));

    // styles block
    QWidget * songStyles = new QWidget(songChracterizationGroup);
    songChracterizationLayout -> addWidget(songStyles);

    stylesLayout = new QVBoxLayout(songStyles);
    songStyles -> setLayout(stylesLayout);

    QPushButton * addStyle = new QPushButton(QStringLiteral("Add Style creteria"), songChracterizationGroup);
    songChracterizationLayout -> addWidget(addStyle);
    connect(addStyle, SIGNAL(clicked(bool)), this, SLOT(onStyleAdd()));

    // moods block
    QWidget * songMoods = new QWidget(songChracterizationGroup);
    songChracterizationLayout -> addWidget(songMoods);

    moodsLayout = new QVBoxLayout(songMoods);
    songMoods -> setLayout(moodsLayout);

    QPushButton * addMood = new QPushButton(QStringLiteral("Add Mood creteria"), songChracterizationGroup);
    songChracterizationLayout -> addWidget(addMood);
    connect(addMood, SIGNAL(clicked(bool)), this, SLOT(onMoodAdd()));
}
