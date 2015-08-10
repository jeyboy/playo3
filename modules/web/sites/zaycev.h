#ifndef ZAYCEV
#define ZAYCEV

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class Zaycev : public IGrabberApi {
    public:
        static Zaycev * instance();
        inline static void close() { delete self; }

        QJsonArray search(QString & predicate, QString & genre, bool popular_items, bool /*by_artist*/, int count) {
            QUrl url;

            if (!predicate.isEmpty()) {
                url = QUrl(baseUrlStr(QStringLiteral("/search.html")));
                url.setQuery(QStringLiteral("query_search=") % predicate); // &page=7
            } else if (!genre.isEmpty())
                return byGenre(genre);
            else if (popular_items)
                return popular();

            if (url.isEmpty()) return QJsonArray();

            QJsonArray json;
            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);

            // need to loop by pagination

            QNetworkReply * response = manager -> getSync(url);

//            {"url":"http://dl.zaycev.net/85673/2745662/rick_ross_-_love_sosa.mp3?dlKind=play&format=json"}

//            if (!tables.isEmpty()) { // at this time table with albums is ignored (second table in the list)
//                Html::Tag * artists_table = 0;
//                Html::Tag * songs_table = 0;

//                for(int i = 0; i < tables.count(); i++) {
//                    Html::Set columns = tables[i] -> find(&table_columns_selector);
//                    if (columns.count() == 3) {
//                        if (columns[0] -> text().isEmpty())
//                            artists_table = tables[i];
//                        else
//                            songs_table = tables[i];
//                    }
//                }

//                if (by_artist && artists_table) {
//                    QHash<QString, QString> artistLinks;
//                    artistsToJson(manager, artists_table -> findLinks(&artistSelector, artistLinks), json);
//                }

//                if (!by_artist && songs_table) {
//                    Html::Set songs = songs_table -> find(&songTrSelector);

//                    if (count < ITEMS_PER_PAGE)
//                        while(songs.size() > count)
//                            songs.removeLast();

//                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
//                        QJsonObject track_obj;

//                        Html::Tag * artist_tag = (*song) -> find(&artistSelector).first();
//                        Html::Tag * track_tag = (*song) -> find(&songSelector).first();
//                        Html::Tag * size_tag = (*song) -> childTag("td", 2);

//                        if (!size_tag) {
//                            Logger::instance() -> writeToStream(
//                                QStringLiteral("Myzuka grabber"),
//                                QStringLiteral("Search: parsing of songs"),
//                                QStringLiteral("Some shit happened"), true
//                            );
//                            return json;
//                        }

//                        QString title = artist_tag -> text() % QStringLiteral(" - ") % track_tag -> text();
//                        track_obj.insert(title_key, title);
//                        track_obj.insert(size_key, prepareSize(size_tag -> text()));
//                        track_obj.insert(refresh_key, baseUrlStr(track_tag -> link()));

//                        json << track_obj;
//                    }
//                }
//            }

            delete response;
            if (isNew) delete manager;
            return json;
        }

        TargetGenres genresList() {
            if (genres.isEmpty()) {
//                QString genresPath = baseUrlStr(QStringLiteral("/Genre/Page"));
//                WebManager * manager = 0;
//                bool isNew = WebManager::valid(manager);

//                for(int page = 1; page < STYLES_MAX_PAGE; page++) {
//                    QUrl url(genresPath % QString::number(page));
//                    QNetworkReply * response = manager -> getSync(QNetworkRequest(url));

//                    Html::Document parser(response);
//                    Html::Set links = parser.find(&linksSelector);

//                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
//                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
//                        genres.addGenre(list[2], list[1].toInt());
//                    }

//                    delete response;
//                    if (links.isEmpty()) break;
//                    QThread::msleep(GRAB_DELAY);
//                }

//                if (isNew) delete manager;
            }

            return genres;
        }

        QJsonArray byGenre(QString genre) {
            QJsonArray json;
//            if (genresList().isEmpty()) genresList();

//            int code = genres.toInt(genre);
//            if (code == genres.defaultInt()) return json;

//            WebManager * manager = 0;
//            bool isNew = WebManager::valid(manager);
//            QString genrePath = baseUrlStr(QStringLiteral("/Genre/%1/%2/Page").arg(QString::number(code), genre));
//            QHash<QString, QString> artistLinks;

//            for(int page = 1; page < MAX_PAGE; page++) {
//                QUrl url(genrePath % QString::number(page));
//                QNetworkReply * response = manager -> getSync(QNetworkRequest(url));
//                Html::Document doc(response);

//                doc.find(&searchTablesSelector).findLinks(&artistSelector, artistLinks);

//                QThread::msleep(GRAB_DELAY); // extra pause
//                delete response;
//            }

//            artistsToJson(manager, artistLinks, json);

//            if (isNew) delete manager;
            return json;
        }

//        QJsonArray byChar(QChar /*target_char*/) { // https://myzuka.org/Artist/5633/G-Playaz/Songs/Page
//            //TODO: realize later
//        }

//        QJsonArray byType(QString /*target_type*/) { // https://myzuka.org/Hits/2014 //https://myzuka.org/Hits/Top100Weekly //https://myzuka.org/Hits/Top100Monthly
//            //TODO: realize later
//        }

        QJsonArray popular() {
            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QJsonArray res;
            toJson(manager -> getSync(QNetworkRequest(QUrl(baseUrlStr()))), res, true);
            if (isNew) delete manager;
            return res;
        }

    protected:
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://zaycev.net") % predicate; }

        bool toJson(QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);

            Html::Set songs = parser.find(".musicset-track-list__items .musicset-track");

            for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                QJsonObject song_obj;

                song_obj.insert(refresh_key, baseUrlStr((*song) -> value(QStringLiteral("data-url"))));
                song_obj.insert(duration_key, Duration::fromSeconds((*song) -> value(QStringLiteral("data-duration")).toInt()));
                //                song_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

                json << song_obj;
            }

//            Html::Document parser(reply);

//            Html::Set set;
//            Html::Tag * tag;
//            Html::Set tracks = parser.find("div[itemprop='tracks']");
//            Html::Selector urlSelector("span[data-url^'/Song']");
//            Html::Selector infoSelector(".data>text");
//            Html::Selector detailsSelector(".details>.time>text");
//            Html::Selector refreshSelector(".details a[href^'/Song']");

//            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
//                QJsonObject track_obj;

//                tag = (*track) -> find(&urlSelector).first();
//                track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
//                track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

//                set = (*track) -> find(&infoSelector);
//                if (!set.isEmpty()) {
//                    track_obj.insert(duration_key, set.first() -> text().section(' ', 0, 0));
//                    track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));
//                }

//                set = (*track) -> find(&detailsSelector);
//                if (!set.isEmpty())
//                    track_obj.insert(size_key, prepareSize(set.first() -> text()));

//                set = (*track) -> find(&refreshSelector);
//                if (!set.isEmpty())
//                    track_obj.insert(refresh_key, baseUrlStr(set.first() -> link()));

//                json << track_obj;
//            }

            if (removeReply) delete reply;
            return !songs.isEmpty();
        }

        QString refresh_postprocess(QNetworkReply * reply) {
            return WebManager::replyToJson(reply).value(QStringLiteral("url")).toString();
        }
    private:
        inline Zaycev() : IGrabberApi() {}
        inline virtual ~Zaycev() {}

        static Zaycev * self;
    };
}

#endif // ZAYCEV
