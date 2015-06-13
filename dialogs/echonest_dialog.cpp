#include "echonest_dialog.h"
#include "ui_echonest_dialog.h"
#include <qdebug.h>

EchonestDialog::EchonestDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::EchonestDialog) {
    ui -> setupUi(this);
}

EchonestDialog::~EchonestDialog() {
    delete ui;
}

void EchonestDialog::on_findArtistSongs_clicked() {
    if (!ui -> findArtistName -> text().isEmpty()) {
        QJsonObject info = EchonestApi::instance() -> artistProfile(ui -> findArtistName -> text());


//        Accordion * accordion = new Accordion(this);
//        accordion -> addItem();

        for(int loop1 = ui -> artistTabs -> count() - 1; loop1 > -1; loop1--) {
            QWidget * widget =  ui -> artistTabs -> widget(loop1);
            ui -> artistTabs -> removeItem(loop1);
            delete widget;
        }

        if (!info.isEmpty()) {
            QJsonArray biographies = info.value("biographies").toArray();
            QJsonArray::Iterator biographie = biographies.begin();

            for(int i = 1; biographie != biographies.end(); biographie++, i++) {
                QLabel * biographie_label = new QLabel((*biographie).toObject().value("text").toString(), ui -> artistTabs);
                biographie_label -> setWordWrap(true);
                ui -> artistTabs -> addItem(biographie_label, "Biographie " + QString::number(i));
            }


            QWidget * statistic = new QWidget(ui -> artistTabs);
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

                l -> addWidget(new QLabel("Genres: " + genresList.join(','), statistic));
            }

            QJsonArray terms = info.value("terms").toArray();

            if (!terms.isEmpty()) {
                QStringList termsList;

                for(QJsonArray::Iterator term = terms.begin(); term != terms.end(); term++) {
                    QJsonObject obj = (*term).toObject();
                    termsList <<
                        (
                            obj.value("name").toString() +
                                "(frequency: " + (obj.value("frequency").toDouble() * 100) + ")" +
                                    "(weight: " + (obj.value("weight").toDouble() * 100) + ")"
                        );
                }

                l -> addWidget(new QLabel("Terms: " + termsList.join(','), statistic));
            }

            ui -> artistTabs -> addItem(statistic, "Statistic");



            QJsonArray news = info.value("news").toArray();

            if (!news.isEmpty()) {
                QWidget * newsBlock = new QWidget(ui -> artistTabs);
                QVBoxLayout * il = new QVBoxLayout(newsBlock);

                for(QJsonArray::Iterator news_item = news.begin(); news_item != news.end(); news_item++) {
                    QJsonObject obj = (*news_item).toObject();
                    QLabel * newsLabel = new QLabel(
                                obj.value("date_found").toString() + "\n" + obj.value("name").toString() + "\n\n" + obj.value("summary").toString()
                                , newsBlock);
                    il -> addWidget(newsLabel);
                }

                ui -> artistTabs -> addItem(newsBlock, "News");
            }

            QJsonArray songs = info.value("songs").toArray();

            if (!songs.isEmpty()) {
                QWidget * songsBlock = new QWidget(ui -> artistTabs);
                QVBoxLayout * il = new QVBoxLayout(songsBlock);

                for(QJsonArray::Iterator song = songs.begin(); song != songs.end(); song++) {
                    QJsonObject obj = (*song).toObject();
                    QLabel * newsLabel = new QLabel(
                                obj.value("title").toString()
                                , songsBlock);
//                    obj.value("name").toString()
                    il -> addWidget(newsLabel);
                }

                ui -> artistTabs -> addItem(songsBlock, "Songs");
            }

            QJsonArray images = info.value("images").toArray();

            if (!images.isEmpty()) {
                QWidget * imagesBlock = new QWidget(ui -> artistTabs);
                QVBoxLayout * il = new QVBoxLayout(imagesBlock);

                for(QJsonArray::Iterator image = images.begin(); image != images.end(); image++) {
                    QLabel * imageLabel = new QLabel(imagesBlock);
                    QUrl url((*image).toObject().value("url").toString());
                    QPixmap pixma = CustomNetworkAccessManager::manager() -> openImage(url);

                    imageLabel -> setPixmap(pixma);
                    il -> addWidget(imageLabel);
                }

                ui -> artistTabs -> addItem(imagesBlock, "Images");
            }
        }
    }
}
