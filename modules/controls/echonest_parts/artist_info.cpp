#include "artist_info.h"

using namespace Controls::Echonest;

ArtistInfo::ArtistInfo(QWidget * parent) : QWidget(parent) {
    setObjectName(QStringLiteral("ArtistInfo"));
    generateLayout();
}

void ArtistInfo::onArtistInfoButtonClicked() {
    if (!artistName -> text().isEmpty()) {
        QJsonObject info = Core::Web::Echonest::Api::obj().artistProfile(artistName -> text());

        artistAccordion -> clear();

        if (!info.isEmpty()) {
            QJsonArray biographies = info.value(QStringLiteral("biographies")).toArray();
            QJsonArray::Iterator biographie = biographies.begin();

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


            QJsonArray genres = info.value(QStringLiteral("genres")).toArray();

            if (!genres.isEmpty()) {
                QStringList genresList;

                for(QJsonArray::Iterator genre = genres.begin(); genre != genres.end(); genre++)
                    genresList << (*genre).toObject().value(QStringLiteral("name")).toString();

                l -> addWidget(new QLabel(QStringLiteral("Genres: \n\t") % genresList.join(QStringLiteral("\n\t")), statistic));
            }

            QJsonArray terms = info.value(QStringLiteral("terms")).toArray();

            if (!terms.isEmpty()) {
                QStringList termsList;

                for(QJsonArray::Iterator term = terms.begin(); term != terms.end(); term++) {
                    QJsonObject obj = (*term).toObject();
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



            QJsonArray news = info.value(QStringLiteral("news")).toArray();

            if (!news.isEmpty()) {
                QWidget * newsBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(newsBlock);

                for(QJsonArray::Iterator news_item = news.begin(); news_item != news.end(); news_item++) {
                    QJsonObject obj = (*news_item).toObject();
                    QLabel * newsLabel = new QLabel(
                                obj.value(QStringLiteral("date_found")).toString() % QStringLiteral("\n") % obj.value(QStringLiteral("name")).toString() % QStringLiteral("\n\n") % obj.value(QStringLiteral("summary")).toString()
                                , newsBlock);
                    newsLabel -> setWordWrap(true);
                    il -> addWidget(newsLabel);
                }

                artistAccordion -> addItem(QStringLiteral("News"), newsBlock);
            }

            QJsonArray songs = info.value(QStringLiteral("songs")).toArray();

            if (!songs.isEmpty()) {
                QWidget * songsBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(songsBlock);
                QHash<QString, bool> songsHash;

                for(QJsonArray::Iterator song = songs.begin(); song != songs.end(); song++) {
                    QJsonObject obj = (*song).toObject();

                    QString str = obj.value(QStringLiteral("title")).toString();

                    if (!songsHash.contains(str)) {
                        QLabel * newsLabel = new QLabel(str, songsBlock);
    //                    obj.value("name").toString()
                        il -> addWidget(newsLabel);
                        songsHash.insert(str, true);
                    }
                }

                artistAccordion -> addItem(QStringLiteral("Songs"), songsBlock);
            }

            QJsonArray images = info.value(QStringLiteral("images")).toArray();

            if (!images.isEmpty()) {
                QWidget * imagesBlock = new QWidget(artistAccordion);
                QVBoxLayout * il = new QVBoxLayout(imagesBlock);

                for(QJsonArray::Iterator image = images.begin(); image != images.end(); image++) {
                    QLabel * imageLabel = new QLabel(imagesBlock);
                    QUrl url((*image).toObject().value(QStringLiteral("url")).toString());
                    QPixmap pixma = Core::Web::Manager::prepare() -> getImage(url);
                    pixma = pixma.scaledToWidth(artistAccordion -> width() - 60, Qt::SmoothTransformation);

                    imageLabel -> setPixmap(pixma);
                    il -> addWidget(imageLabel);
                }

                artistAccordion -> addItem(QStringLiteral("Images"), imagesBlock);
            }
        }
    }
}

void ArtistInfo::generateLayout() {
    QGridLayout * artistInfoLayout = new QGridLayout();

    artistName = new QLineEdit(this);
    artistInfoLayout -> addWidget(artistName, 0, 0);

    connect(artistName, SIGNAL(returnPressed()), this, SLOT(onArtistInfoButtonClicked()));

    QPushButton * artistInfoButton = new QPushButton(QStringLiteral("Find Info"), base);
    connect(artistInfoButton, SIGNAL(clicked()), this, SLOT(onArtistInfoButtonClicked()));
    artistInfoLayout -> addWidget(artistInfoButton, 0, 1);

    artistAccordion = new Controls::Accordion(base);
    artistInfoLayout -> addWidget(artistAccordion, 1, 0, 10, 2);

    setLayout(artistInfoLayout);
}
