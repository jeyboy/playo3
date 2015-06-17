#include "echonest_dialog.h"
#include "ui_echonest_dialog.h"
#include <qdebug.h>

#include "double_slider/qxtspanslider.h"

EchonestDialog::EchonestDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::EchonestDialog) {
    ui -> setupUi(this);

    Accordion * accordion = new Accordion(this);

//    QxtSpanSlider * slider = new QxtSpanSlider(Qt::Horizontal, this);
//    slider -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
//    layout() -> addWidget(slider);

    layout() -> addWidget(accordion);

    QWidget * tab1 = accordion -> addItem("Artist Info");
    QGridLayout * gl = new QGridLayout();

    artistName = new QLineEdit(tab1);
    gl -> addWidget(artistName, 0, 0);

    connect(artistName, SIGNAL(returnPressed()), this, SLOT(onArtistInfoButtonClicked()));

    QPushButton * artistInfoButton = new QPushButton("Find Info", tab1);
    connect(artistInfoButton, SIGNAL(clicked()), this, SLOT(onArtistInfoButtonClicked()));
    gl -> addWidget(artistInfoButton, 0, 1);

    artistAccordion = new Accordion(tab1);
    gl -> addWidget(artistAccordion, 1, 0, 10, 2);

    tab1 -> setLayout(gl);


    QWidget * tab2 = accordion -> addItem("Playlist generation");
    QVBoxLayout * gl2 = new QVBoxLayout(tab2);

    Accordion * playlistAccordion = new Accordion(tab2);
    gl2 -> addWidget(playlistAccordion);


    QWidget * playlistBasic = playlistAccordion -> addItem("Base Playlist");

    basicPlaylistGeneration(playlistBasic);
}

EchonestDialog::~EchonestDialog() {
    delete ui;
}

void EchonestDialog::onArtistInfoButtonClicked() {
    if (!artistName -> text().isEmpty()) {
        QJsonObject info = EchonestApi::instance() -> artistProfile(artistName -> text());

        artistAccordion -> clear();

        if (!info.isEmpty()) {
            QJsonArray biographies = info.value("biographies").toArray();
            QJsonArray::Iterator biographie = biographies.begin();

            for(int i = 1; biographie != biographies.end(); biographie++, i++) {
                QLabel * biographie_label = new QLabel((*biographie).toObject().value("text").toString(), artistAccordion);
                biographie_label -> setWordWrap(true);
                artistAccordion -> addItem("Biographie " + QString::number(i), biographie_label);
            }


            QWidget * statistic = new QWidget(artistAccordion);
            QVBoxLayout * l = new QVBoxLayout(statistic);
            l -> addWidget(new QLabel(
                "Familiarity: " + QString::number((info.value("familiarity").toDouble() * 100)) + "%",
                statistic)
            );

            l -> addWidget(new QLabel(
                "Hotttnesss: " + QString::number((info.value("hotttnesss").toDouble() * 100)) + "%",
                statistic)
            );

            l -> addWidget(new QLabel(
                "Start career from: " + QString::number(info.value("years_active").toArray().first().toObject().value("start").toInt()),
                statistic)
            );


            QJsonArray genres = info.value("genres").toArray();

            if (!genres.isEmpty()) {
                QStringList genresList;

                for(QJsonArray::Iterator genre = genres.begin(); genre != genres.end(); genre++)
                    genresList << (*genre).toObject().value("name").toString();

                l -> addWidget(new QLabel("Genres: \n\t" + genresList.join("\n\t"), statistic));
            }

            QJsonArray terms = info.value("terms").toArray();

            if (!terms.isEmpty()) {
                QStringList termsList;

                for(QJsonArray::Iterator term = terms.begin(); term != terms.end(); term++) {
                    QJsonObject obj = (*term).toObject();
                    termsList <<
                        (
                            obj.value("name").toString() + "\n" +
                                "\t\tfrequency: " + QString::number(obj.value("frequency").toDouble() * 100) + "%\n" +
                                "\t\tweight: " + QString::number(obj.value("weight").toDouble() * 100) + "%"
                        );
                }

                QLabel * termsLabel = new QLabel("Terms: \n\t" + termsList.join("\n\t"), statistic);
                termsLabel -> setWordWrap(true);
                l -> addWidget(termsLabel);
            }

            artistAccordion -> addItem("Statistic", statistic);



            QJsonArray news = info.value("news").toArray();

            if (!news.isEmpty()) {
                QWidget * newsBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(newsBlock);

                for(QJsonArray::Iterator news_item = news.begin(); news_item != news.end(); news_item++) {
                    QJsonObject obj = (*news_item).toObject();
                    QLabel * newsLabel = new QLabel(
                                obj.value("date_found").toString() + "\n" + obj.value("name").toString() + "\n\n" + obj.value("summary").toString()
                                , newsBlock);
                    newsLabel -> setWordWrap(true);
                    il -> addWidget(newsLabel);
                }

                artistAccordion -> addItem("News", newsBlock);
            }

            QJsonArray songs = info.value("songs").toArray();

            if (!songs.isEmpty()) {
                QWidget * songsBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(songsBlock);
                QHash<QString, bool> songsHash;

                for(QJsonArray::Iterator song = songs.begin(); song != songs.end(); song++) {
                    QJsonObject obj = (*song).toObject();

                    QString str = obj.value("title").toString();

                    if (!songsHash.contains(str)) {
                        QLabel * newsLabel = new QLabel(str, songsBlock);
    //                    obj.value("name").toString()
                        il -> addWidget(newsLabel);
                        songsHash.insert(str, true);
                    }
                }

                artistAccordion -> addItem("Songs", songsBlock);
            }

            QJsonArray images = info.value("images").toArray();

            if (!images.isEmpty()) {
                QWidget * imagesBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(imagesBlock);

                for(QJsonArray::Iterator image = images.begin(); image != images.end(); image++) {
                    QLabel * imageLabel = new QLabel(imagesBlock);
                    QUrl url((*image).toObject().value("url").toString());
                    QPixmap pixma = CustomNetworkAccessManager::manager() -> openImage(url);
                    pixma = pixma.scaledToWidth(artistAccordion -> width() - 60, Qt::SmoothTransformation);

                    imageLabel -> setPixmap(pixma);
                    il -> addWidget(imageLabel);
                }

                artistAccordion -> addItem("Images", imagesBlock);
            }
        }
    }
}

void EchonestDialog::onBasicPlaylistGenerateClicked() {
    //    QJsonArray playlistBasic(QString type = QString(), QStringList artists = QStringList(),
    //                             QStringList genres = QStringList(), QStringList songs_ids = QStringList(), int limit = 100) {
}

void EchonestDialog::basicPlaylistGeneration(QWidget * base) {
    QVBoxLayout * layoutSimilar = new QVBoxLayout(base);

    QWidget * playlistType = new QWidget(base);

    QGroupBox * playlist_type = new QGroupBox("Playlist Type", playlistType);
    QHBoxLayout * playlist_type_layout = new QHBoxLayout(playlist_type);
    artistTypeCheck = new QCheckBox("By similar artists", playlist_type);
    playlist_type_layout -> addWidget(artistTypeCheck);
    genreTypeCheck = new QCheckBox("By genres", playlist_type);
    playlist_type_layout -> addWidget(genreTypeCheck);

    layoutSimilar -> addWidget(playlist_type);

    QGroupBox * artist_type_fields = new QGroupBox("Artists for query", playlistType);
    QVBoxLayout * artists_layout = new QVBoxLayout(artist_type_fields);
    for(int i = 0; i < 5; i++) {
        QLineEdit * edit = new QLineEdit(artist_type_fields);
        basicPlaylistArtists << edit;
        artists_layout -> addWidget(edit);
    }

    layoutSimilar -> addWidget(artist_type_fields);

    QGroupBox * genre_type_fields = new QGroupBox("Genres for query", playlistType);
    QStringList genres = genresList();
    QVBoxLayout * genres_layout = new QVBoxLayout(genre_type_fields);
    for(int i = 0; i < 5; i++) {
        QComboBox * combo = new QComboBox(genre_type_fields);
        combo -> addItems(genres);
        basicPlaylistGenres << combo;
        genres_layout -> addWidget(combo);
    }

    layoutSimilar -> addWidget(genre_type_fields);

    QPushButton * basicPlaylistStart = new QPushButton("Generate basic playlist", base);
    connect(basicPlaylistStart, SIGNAL(clicked()), this, SLOT(onBasicPlaylistGenerateClicked()));
    layoutSimilar -> addWidget(basicPlaylistStart);
}

QStringList EchonestDialog::genresList() {
    QStringList styles; styles << "";

    QJsonArray genres = EchonestApi::instance() -> artistGenresForSearch();
    for(QJsonArray::Iterator genre = genres.begin(); genre != genres.end(); genre++)
        styles.append((*genre).toObject().value("name").toString());

    return styles;
}
QStringList EchonestDialog::moodsList() {
    QStringList moodsList;

    QJsonArray moods = EchonestApi::instance() -> artistMoodsForSearch();
    for(QJsonArray::Iterator mood = moods.begin(); mood != moods.end(); mood++)
        moodsList.append((*mood).toObject().value("name").toString());

    return moodsList;
}
