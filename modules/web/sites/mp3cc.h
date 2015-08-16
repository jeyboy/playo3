#ifndef MP3CC
#define MP3CC

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 47
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class Mp3cc : public IGrabberApi {
    public:
        static Mp3cc * instance();
        inline static void close() { delete self; }

        TargetGenres genresList() { // manual init at this time
            if (genres.isEmpty()) {
                genres.addGenre(QStringLiteral("Pop"), QStringLiteral("pop"));
                genres.addGenre(QStringLiteral("Rock"), QStringLiteral("rock"));
                genres.addGenre(QStringLiteral("Rap"), QStringLiteral("rap"));
                genres.addGenre(QStringLiteral("Alternative"), QStringLiteral("alternative"));
                genres.addGenre(QStringLiteral("Rhythm and Blues"), QStringLiteral("rnb"));
                genres.addGenre(QStringLiteral("Rhythm & Blues"), QStringLiteral("rnb"));
                genres.addGenre(QStringLiteral("Metal"), QStringLiteral("metal"));
                genres.addGenre(QStringLiteral("Reggae"), QStringLiteral("reggae"));
                genres.addGenre(QStringLiteral("Jazz"), QStringLiteral("jazz"));
                genres.addGenre(QStringLiteral("Country"), QStringLiteral("country"));
                genres.addGenre(QStringLiteral("Chanson"), QStringLiteral("chanson"));
                genres.addGenre(QStringLiteral("Blues"), QStringLiteral("blues"));
                genres.addGenre(QStringLiteral("Dance"), QStringLiteral("dance"));
                genres.addGenre(QStringLiteral("Estrada"), QStringLiteral("estrada"));
                genres.addGenre(QStringLiteral("Easy Listening"), QStringLiteral("relax"));
                genres.addGenre(QStringLiteral("Folk"), QStringLiteral("folk"));
                genres.addGenre(QStringLiteral("Electronic"), QStringLiteral("electronic"));
                genres.addGenre(QStringLiteral("Drum & Bass"), QStringLiteral("dnb"));
                genres.addGenre(QStringLiteral("Soundtrack"), QStringLiteral("soundtrack"));
                genres.addGenre(QStringLiteral("Classical"), QStringLiteral("classical"));
            }

            return genres;
        }

//        QJsonArray byGenre(QString genre, int genre_id) { // http://zaycev.net/genres/shanson/index.html
//            QJsonArray json;
//            if (genresList().isEmpty()) genresList();

//            genre = genres.toString(genre_id);
//            if (genre.isEmpty()) return json;

//            QString url_str = baseUrlStr(QStringLiteral("/genres/%1/index_%2.html").arg(genre, page_offset_key));
//            lQuery(url_str, json, songs1, MAX_PAGE);

//            return json;
//        }

        // rus letters has specific presentation
//        QJsonArray byChar(QChar /*target_char*/) { http://zaycev.net/artist/letter-rus-zh-more.html?page=1
//            //TODO: realize later
//        }

//        // one page contains 30 albums
//        QJsonArray byType(ByTypeArg target_type) { //http://zaycev.net/musicset/more.html?page=1
//            switch (target_type) { // need to modify grab processing of folder support in model
//                case sets: break; // http://zaycev.net/musicset/more.html?page=2
//                case soundtracks: break; // http://zaycev.net/musicset/soundtrack/more.html?page=2
//                case by_genres: break; // http://zaycev.net/musicset/zhanry/more.html?page=2
//                case by_years: break; // http://zaycev.net/musicset/years/more.html?page=2
//                case other: break; // http://zaycev.net/musicset/other/more.html?page=2
//                case fresh: break; // http://zaycev.net/new/more.html?page=2
//                default: return QJsonArray();
//            }
//            //TODO: stop if result not contains elements
//        }

        QJsonArray popular() { return sQuery(QUrl(baseUrlStr()), songs1); }

    protected:
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3.cc") % predicate; }

        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find(".playlist .track");
                    Html::Selector titles_selector(".playlist-name a");
                    Html::Selector down_selector("a.playlist-btn-down");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject song_obj;

                        song_obj.insert(url_key, (*song) -> find(&down_selector).link()); // alt (*song) -> value(QStringLiteral("data-mp3")).toString()
                        song_obj.insert(duration_key, Duration::fromMillis((*song) -> value(QStringLiteral("data-duration")).toInt()));
                        song_obj.insert(skip_info_key, true);

                        Html::Set titles = (*song) -> find(&titles_selector);
                        QString title = titles.first() -> text() % QStringLiteral(" - ") % titles.last() -> text();
                        song_obj.insert(title_key, title);

                        qDebug() << song_obj;
                        json << song_obj;
                    }

                    result = !songs.isEmpty();
                }

                default: ;
            }

            if (removeReply) delete reply;
            return result;
        }

//        inline QString refresh_postprocess(QNetworkReply * reply) {
//            return WebManager::replyToJson(reply).value(QStringLiteral("url")).toString();
//        }

        QJsonArray search_postprocess(QString & predicate, bool by_artist, bool by_song, QString & /*genre*/, int /*genre_id*/, int count) { // 47 items per page
            QString url_str = baseUrlStr(QStringLiteral("/search/%1/%2/page/%3/")).arg(
                QString(by_artist ? 'a' : (by_song ? 't' : 'f')),
                QUrl::toPercentEncoding(predicate),
                page_offset_key
            );

            QJsonArray json;
            lQuery(url_str, json, songs1, MAX_PAGE);

            while(json.size() > count)
                json.removeLast();

            return json;
        }
    private:
        inline Mp3cc() : IGrabberApi() {}
        inline virtual ~Mp3cc() {}

        static Mp3cc * self;
    };
}

#endif // MP3CC
