#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class MyzukaAlbum : public IGrabberApi {
        const QString data_url_token, title_token, search_path_token, search_predicate_token;
        const Html::Selector searchTablesSelector, songTrSelector, artistSelector, songSelector, linksSelector, table_columns_selector;
    public:
        static MyzukaAlbum * instance();
        inline static void close() { delete self; }

        QJsonArray search(QString & predicate, QString & genre, bool popular_items, bool by_artist, int count) {
            QUrl url;

            if (!predicate.isEmpty()) {
                url = QUrl(baseUrlStr(search_path_token));
                url.setQuery(search_predicate_token % predicate);
            } else if (!genre.isEmpty())
                return byGenre(genre);
            else if (popular_items)
                return popular();

            if (url.isEmpty()) return QJsonArray();

            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QNetworkReply * response = manager -> getSync(QNetworkRequest(url));

            QJsonArray json;
            Html::Document parser(response);

            Html::Set tables = parser.find(&searchTablesSelector);

            if (!tables.isEmpty()) { // at this time table with albums is ignored (second table in the list)
                Html::Tag * artists_table = 0;
                Html::Tag * songs_table = 0;

                for(int i = 0; i < tables.count(); i++) {
                    Html::Set columns = tables[i] -> find(&table_columns_selector);
                    if (columns.count() == 3) {
                        if (columns[0] -> text().isEmpty())
                            artists_table = tables[i];
                        else
                            songs_table = tables[i];
                    }
                }

                if (by_artist && artists_table) {
                    QHash<QString, QString> artistLinks;
                    artistsToJson(manager, artists_table -> findLinks(&artistSelector, artistLinks), json);
                }

                if (!by_artist && songs_table) {
                    Html::Set songs = songs_table -> find(&songTrSelector);

                    if (count < ITEMS_PER_PAGE)
                        while(songs.size() > count)
                            songs.removeLast();

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject track_obj;

                        Html::Tag * artist_tag = (*song) -> find(&artistSelector).first();
                        Html::Tag * track_tag = (*song) -> find(&songSelector).first();
                        Html::Tag * size_tag = (*song) -> childTag("td", 2);

                        if (!size_tag) {
                            Logger::instance() -> writeToStream(
                                QStringLiteral("Myzuka grabber"),
                                QStringLiteral("Search: parsing of songs"),
                                QStringLiteral("Some shit happened"), true
                            );
                            return json;
                        }

                        QString title = artist_tag -> text() % QStringLiteral(" - ") % track_tag -> text();
                        track_obj.insert(title_key, title);
                        track_obj.insert(size_key, prepareSize(size_tag -> text()));
                        track_obj.insert(refresh_key, baseUrlStr(track_tag -> link()));

                        json << track_obj;
                    }
                }
            }

            delete response;
            if (isNew) delete manager;
            return json;
        }

        TargetGenres genresList() {
            if (genres.isEmpty()) {
                QString genresPath = baseUrlStr(QStringLiteral("/Genre/Page"));
                WebManager * manager = 0;
                bool isNew = WebManager::valid(manager);

                for(int page = 1; page < STYLES_MAX_PAGE; page++) {
                    QUrl url(genresPath % QString::number(page));
                    QNetworkReply * response = manager -> getSync(QNetworkRequest(url));

                    Html::Document parser(response);
                    Html::Set links = parser.find(&linksSelector);

                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                        genres.addGenre(list[2], list[1].toInt());
                    }

                    delete response;
                    if (links.isEmpty()) break;
                    QThread::msleep(GRAB_DELAY);
                }

                if (isNew) delete manager;
            }

            return genres;
        }

        // artists by genre
        QJsonArray byGenre(QString genre, int genre_id) { // https://myzuka.org/Genre/92/8-Bit https://myzuka.org/Genre/11/Pop/Page2
            QJsonArray json;
            if (genresList().isEmpty()) genresList();

            genre_id = genres.toInt(genre);
            if (genre_id == genres.defaultInt()) return json;

            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QString genrePath = baseUrlStr(QStringLiteral("/Genre/%1/%2/Page").arg(QString::number(genre_id), genre));
            QHash<QString, QString> artistLinks;

            for(int page = 1; page < MAX_PAGE; page++) {
                QUrl url(genrePath % QString::number(page));
                QNetworkReply * response = manager -> getSync(QNetworkRequest(url));
                Html::Document doc(response);

                doc.find(&searchTablesSelector).findLinks(&artistSelector, artistLinks);

                QThread::msleep(GRAB_DELAY); // extra pause
                delete response;
            }

            artistsToJson(manager, artistLinks, json);

            if (isNew) delete manager;
            return json;
        }

//        QJsonArray byChar(QChar /*target_char*/) { // https://myzuka.org/Artist/5633/G-Playaz/Songs/Page
//            //TODO: realize later
//        }

//        QJsonArray byType(ByTypeArg /*target_type*/) { // https://myzuka.org/Hits/2014 //https://myzuka.org/Hits/Top100Weekly //https://myzuka.org/Hits/Top100Monthly
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
        inline QString prepareSize(QString size) {
            return size.trimmed().replace("Мб", "Mb").replace("Кб", "Kb");
        }

        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("https://myzuka.org") % predicate; }
        void artistsToJson(WebManager * manager, QHash<QString, QString> & artists, QJsonArray & json) {
            for(QHash<QString, QString>::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                QString artistPage = artist.key() % QStringLiteral("/Songs/Page");

                for(int page = 1; page < MAX_PAGES_PER_ARTIST; page++) {
                    if (!toJson(manager -> getSync(QNetworkRequest(QUrl(baseUrlStr(artistPage % QString::number(page))))), json, true))
                        break;
                    QThread::msleep(GRAB_DELAY);
                }
            }
        }

        bool toJson(QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);

            Html::Set set;
            Html::Tag * tag;
            Html::Set tracks = parser.find("div[itemprop='tracks']");
            Html::Selector urlSelector("span[data-url^'/Song']");
            Html::Selector infoSelector(".data>text");
            Html::Selector detailsSelector(".details>.time>text");
            Html::Selector refreshSelector(".details a[href^'/Song']");

            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                QJsonObject track_obj;

                tag = (*track) -> find(&urlSelector).first();
                track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
                track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

                set = (*track) -> find(&infoSelector);
                if (!set.isEmpty()) {
                    track_obj.insert(duration_key, set.first() -> text().section(' ', 0, 0));
                    track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));
                }

                set = (*track) -> find(&detailsSelector);
                if (!set.isEmpty())
                    track_obj.insert(size_key, prepareSize(set.first() -> text()));

                set = (*track) -> find(&refreshSelector);
                if (!set.isEmpty())
                    track_obj.insert(refresh_key, baseUrlStr(set.first() -> link()));

                json << track_obj;
            }

            if (removeReply) delete reply;
            return !tracks.isEmpty();
        }

        QString refresh_postprocess(QNetworkReply * reply) {
            Html::Document parser(reply);

            Html::Selector trackSelector(".options a[itemprop='audio']");
            Html::Set tracks = parser.find(&trackSelector);

            if (tracks.isEmpty())
                return QString();
            else
                return baseUrlStr(tracks.first() -> link());
        }
    private:
        inline MyzukaAlbum() : IGrabberApi(), data_url_token(QStringLiteral("data-url")),
            title_token(QStringLiteral("title")), search_path_token(QStringLiteral("/Search")),
            search_predicate_token(QStringLiteral("searchText=")), searchTablesSelector(Html::Selector(".content table")),
            songTrSelector(Html::Selector("a[href^'/Song']<tr")), artistSelector(Html::Selector("td a[href^'/Artist']")),
            songSelector(Html::Selector("a[href^'/Song']")), linksSelector(Html::Selector("table a[href^'/Genre/']")),
            table_columns_selector("tr th") { }

        inline virtual ~MyzukaAlbum() {}

        static MyzukaAlbum * self;
    };
}

#endif // MYZUKA_ALBUM
