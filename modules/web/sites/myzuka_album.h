#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class MyzukaAlbum : public IGrabberApi {
        const QString data_url_token, title_token, search_path_token, search_predicate_token;
        const Html::Selector searchTablesSelector, songTrSelector, artistSelector, songSelector, linksSelector;
    public:
        static MyzukaAlbum * instance();
        inline static void close() { delete self; }

        QJsonArray search(QString & predicate, QString & genre, bool popular_items, int count) {
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
                if (count > 10) {
                    Html::Tag * artists_table = tables.first();
                    artistsToJson(manager, artists_table -> find(&artistSelector), json);
                }

                Html::Tag * songs_table = tables.last();
                Html::Set songs = songs_table -> find(&songTrSelector);
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
                    track_obj.insert(size_key, size_tag -> text().section(' ', 0, 0));

                    json << track_obj;
                }
            }

            delete response;
            if (isNew) delete manager;
            return json;
        }

        TargetGenres genresList() {
            if (genres.isEmpty()) {
                QString genresPath = baseUrlStr(QStringLiteral("Genre/Page"));
                WebManager * manager = 0;
                bool isNew = WebManager::valid(manager);

                for(int page = 1; page < STYLES_MAX_PAGE; page++) {
                    QNetworkReply * response = manager -> getSync(QNetworkRequest(QUrl(genresPath % QString::number(page))));

                    Html::Document parser(response);
                    Html::Set links = parser.find(&linksSelector);

                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                        genres.addGenre(list[2], list[1].toInt());
                    }

                    delete response;
                    if (links.isEmpty()) break;
                }

                if (isNew) delete manager;
            }

            return genres;
        }

        // artists by genre
        QJsonArray byGenre(QString genre) { // https://myzuka.org/Genre/92/8-Bit https://myzuka.org/Genre/11/Pop/Page2
            QJsonArray json;
            if (genresList().isEmpty()) genresList();

            int code = genres.toInt(genre);
            if (code == genres.defaultInt()) return json;


            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QString genrePath = baseUrlStr(QStringLiteral("Genre/%1/%2/Page").arg(QString::number(code), genre));

            for(int page = 1; page < MAX_PAGE; page++) {
                QNetworkReply * response = manager -> getSync(QNetworkRequest(QUrl(genrePath % QString::number(page))));
                artistsToJson(manager, Html::Document(response).find(QStringLiteral(".content table>tbody td a[href^'/Artist']")), json);
                delete response;
            }

            if (isNew) delete manager;
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
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("https://myzuka.org") % predicate; }
        void artistsToJson(WebManager * manager, Html::Set artists, QJsonArray & json) {
            for(Html::Set::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                QString artistPage = (*artist) -> link() % QStringLiteral("/Songs/Page");

                for(int page = 1; page < MAX_PAGES_PER_ARTIST; page++)
                    if (!toJson(manager -> getSync(QNetworkRequest(QUrl(baseUrlStr(artistPage % QString::number(page))))), json, true))
                        break;
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
                    track_obj.insert(size_key, set.first() -> text().section(' ', 0, 0));

                set = (*track) -> find(&refreshSelector);
                if (!set.isEmpty())
                    track_obj.insert(refresh_key, set.first() -> link());

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
            title_token(QStringLiteral("title")), search_path_token(QStringLiteral("Search")),
            search_predicate_token(QStringLiteral("searchText=")), searchTablesSelector(Html::Selector(".content table>tbody]")),
            songTrSelector(Html::Selector("a[href^'/Song']<tr")), artistSelector(Html::Selector("td a[href^'/Artist']")),
            songSelector(Html::Selector("a[href^'/Song']")), linksSelector(Html::Selector("a[href^'/Genre/']")) { }

        inline virtual ~MyzukaAlbum() {}

        static MyzukaAlbum * self;
    };
}

#endif // MYZUKA_ALBUM
