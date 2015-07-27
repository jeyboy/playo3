#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

namespace Grabber {
    class MyzukaAlbum : public IGrabberApi {
        const QString data_url_token = QStringLiteral("data-url");
        const QString title_token = QStringLiteral("title");

        const QString search_path_token = QStringLiteral("Search");
        const QString search_predicate_token = QStringLiteral("searchText=");
    public:
        static MyzukaAlbum * instance();
        inline static void close() { delete self; }

        QJsonArray search(QString & predicate, QString & genre, bool popular_items, int count) {
            QUrl url;

            if (!predicate.isEmpty()) {
                url = QUrl(baseUrlStr(search_path_token));
                url.setQuery(search_predicate_token % predicate);
            } else if (!genre.isEmpty()) {
                if (genresList().isEmpty()) genresList();

                int code = genres.toInt(genre);
                if (code != genres.defaultInt())
                    return byGenre(genre, code);
            }

            if (url.isEmpty()) {
                if (popular_items)
                    return popular();
                else return QJsonArray();
            }

            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QNetworkReply * response = manager -> getSync(QNetworkRequest(url));

            QJsonArray track_ar;
            Html::Document parser(response);

            Html::Set tables = parser.find(".content table>tbody]");

            Html::Selector songTrSelector("a[href^'/Song']<tr");
            Html::Selector artistSelector("td a[href^'/Artist']");

            Html::Selector songSelector("a[href^'/Song']");

            if (!tables.isEmpty()) { // at this time table with albums is ignored
                if (count > 10) {
                    Html::Tag * artists_table = tables.first();
                    Html::Set artists = artists_table -> find(&artistSelector);
                    for(Html::Set::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                        QString artistPage = (*artist) -> link() % QStringLiteral("/Songs/Page");

                        for(int page = 1; page < MAX_PAGES_PER_ARTIST; page++)
                            if (!toJson(manager -> getSync(QNetworkRequest(QUrl(baseUrlStr(artistPage % QString::number(page))))), track_ar, true))
                                break;
                    }
                }

                Html::Tag * songs_table = tables.last();
                Html::Set songs = songs_table -> find(&songTrSelector);
                for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                    QJsonObject track_obj;

                    Html::Tag * artist_tag = (*song) -> find(&artistSelector).first();
                    Html::Tag * track_tag = (*song) -> find(&songSelector).first();
                    Html::Tag * size_tag = (*song) -> childTag("td", 2);

                    if (!size_tag) {
                        Logger::instance() -> writeToStream("Myzuka grabber", "Search: parsing of songs", "Some shit happened", true);
                        return track_ar;
                    }

                    QString title = artist_tag -> text() % QStringLiteral(" - ") % track_tag -> text();
                    track_obj.insert(title_key, title);
                    track_obj.insert(size_key, size_tag -> text().section(' ', 0, 0));

                    track_ar << track_obj;
                }
            }

            delete response;
            if (isNew) delete manager;
            return track_ar;
        }

        TargetGenres genresList() {
            if (genres.isEmpty()) {
                QString genresPath = baseUrlStr("Genre/Page");
                WebManager * manager = 0;
                bool isNew = WebManager::valid(manager);

                Html::Selector linksSelector("a[href^'/Genre/']");

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
        QJsonArray byGenre(QString genre, int genre_code = 0) { // https://myzuka.org/Genre/92/8-Bit https://myzuka.org/Genre/11/Pop/Page2
            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QJsonArray track_ar;
            QString genrePath = baseUrlStr("Genre/" % QString::number(genre_code) % "/" % genre % "/Page");

            for(int page = 1; page < MAX_PAGE; page++) {
                QNetworkReply * response = manager -> getSync(QNetworkRequest(QUrl(genrePath % QString::number(page))));
                Html::Document parser(response);

                Html::Set artists = parser.find(".content table>tbody td a[href^'/Artist']");
                for(Html::Set::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                    QString artistPage = (*artist) -> link() % QStringLiteral("/Songs/Page");

                    for(int page = 1; page < MAX_PAGES_PER_ARTIST; page++)
                        if (!toJson(manager -> getSync(QNetworkRequest(QUrl(baseUrlStr(artistPage % QString::number(page))))), track_ar, true))
                            break;
                }

                delete response;
            }

            if (isNew) delete manager;
            return track_ar;
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
        inline MyzukaAlbum() : IGrabberApi() { }
        inline virtual ~MyzukaAlbum() {}

        static MyzukaAlbum * self;
    };
}

#endif // MYZUKA_ALBUM
