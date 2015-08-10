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

        QJsonArray search(QString & predicate, QString & genre, int genre_id, bool popular_items, bool /*by_artist*/, int count) {
            QUrl url;

            if (!predicate.isEmpty()) {
                url = QUrl(baseUrlStr(QStringLiteral("/search.html")));
                url.setQuery(QStringLiteral("query_search=") % predicate); // &page=7
            } else if (!genre.isEmpty())
                return byGenre(genre, genre_id);
            else if (popular_items)
                return popular();

            if (url.isEmpty()) return QJsonArray();

            QJsonArray json;
            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);

            // need to loop by pagination

            QNetworkReply * response = manager -> getSync(url);
            toJson(response, json, true);

            while(json.size() > count)
                json.removeLast();

            if (isNew) delete manager;
            return json;
        }

        TargetGenres genresList() { // manual init at this time
            if (genres.isEmpty()) {
                genres.addGenre(QStringLiteral("pop"), 13);
                genres.addGenre(QStringLiteral("rock"), 17);
                genres.addGenre(QStringLiteral("rap"), 15);
                genres.addGenre(QStringLiteral("alternative"), 20);
                genres.addGenre(QStringLiteral("electronic"), 52);
                genres.addGenre(QStringLiteral("shanson"), 102);
                genres.addGenre(QStringLiteral("soundtrack"), 24);
                genres.addGenre(QStringLiteral("metal"), 9);
                genres.addGenre(QStringLiteral("classical"), 32);
                genres.addGenre(QStringLiteral("dance"), 3);
                genres.addGenre(QStringLiteral("easy"), 98);
                genres.addGenre(QStringLiteral("rnb"), 14);
                genres.addGenre(QStringLiteral("jazz"), 8);
                genres.addGenre(QStringLiteral("reggae"), 16);
                genres.addGenre(QStringLiteral("other"), 12);
            }

            return genres;
        }

        QJsonArray byGenre(QString genre, int genre_id) { // http://zaycev.net/genres/shanson/index.html
            QJsonArray json;
            if (genresList().isEmpty()) genresList();

            genre = genres.toString(genre_id);
            if (genre.isEmpty()) return json;

            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);

            for(int page = 1; page < MAX_PAGE; page++) { // add pagination end check
                QUrl url(baseUrlStr(QStringLiteral("/genres/%1/index_%2.html").arg(genre, QString::number(page))));
                toJson(manager -> getSync(url), json, true);
                QThread::msleep(GRAB_DELAY); // extra pause
            }

            if (isNew) delete manager;
            return json;
        }

        // rus letters has specific presentation
//        QJsonArray byChar(QChar /*target_char*/) { http://zaycev.net/artist/letter-rus-zh-more.html?page=1
//            //TODO: realize later
//        }

        QJsonArray byType(ByTypeArg target_type) { http://zaycev.net/musicset/more.html?page=1
            //TODO: realize later
        }

        QJsonArray popular() {
            WebManager * manager = 0;
            bool isNew = WebManager::valid(manager);
            QJsonArray res;
            toJson(manager -> getSync(QUrl(baseUrlStr())), res, true);
            if (isNew) delete manager;
            return res;
        }

    protected:
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://zaycev.net") % predicate; }

        bool toJson(QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);

            Html::Set songs = parser.find(".musicset-track-list__items .musicset-track"); //.track-is-banned // banned track is not playable for some countries

            for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                QJsonObject song_obj;

                song_obj.insert(refresh_key, baseUrlStr((*song) -> value(QStringLiteral("data-url"))));
                song_obj.insert(duration_key, Duration::fromSeconds((*song) -> value(QStringLiteral("data-duration")).toInt()));

                QString artist = (*song) -> find(".musicset-track__artist a").text();
                QString title = (*song) -> find(".musicset-track__track-name a").text();
                title = artist % QStringLiteral(" - ") % title;
                song_obj.insert(title_key, title);

                json << song_obj;
            }

            if (removeReply) delete reply;
            return !songs.isEmpty();
        }

        // {"url":"http://dl.zaycev.net/85673/2745662/rick_ross_-_love_sosa.mp3?dlKind=play&format=json"}
        inline QString refresh_postprocess(QNetworkReply * reply) {
            return WebManager::replyToJson(reply).value(QStringLiteral("url")).toString();
        }
    private:
        inline Zaycev() : IGrabberApi() {}
        inline virtual ~Zaycev() {}

        static Zaycev * self;
    };
}

#endif // ZAYCEV
