#ifndef MP3PM
#define MP3PM

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 47
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class Mp3pm : public IGrabberApi {
    public:
        static Mp3pm * instance();
        inline static void close() { delete self; }

        TargetGenres genresList() { // manual init at this time
            if (genres.isEmpty()) {
//                genres.addGenre(QStringLiteral("Rock"), QStringLiteral("rock"));
//                genres.addGenre(QStringLiteral("rap"), 15);
//                genres.addGenre(QStringLiteral("HipHop"), QStringLiteral("rap"));
//                genres.addGenre(QStringLiteral("Easy Listening"), QStringLiteral("relax"));
//                genres.addGenre(QStringLiteral("Dance"), QStringLiteral("dance_house"));
//                genres.addGenre(QStringLiteral("House"), QStringLiteral("dance_house"));
//                genres.addGenre(QStringLiteral("House"), QStringLiteral("instrumental"));

//                genres.addGenre(QStringLiteral("Pop"), QStringLiteral("pop"));
//                genres.addGenre(QStringLiteral("Alternative"), QStringLiteral("alternative"));
//                genres.addGenre(QStringLiteral("Rhythm and Blues"), QStringLiteral("rnb"));
//                genres.addGenre(QStringLiteral("Rhythm & Blues"), QStringLiteral("rnb"));
//                genres.addGenre(QStringLiteral("Metal"), QStringLiteral("metal"));
//                genres.addGenre(QStringLiteral("Reggae"), QStringLiteral("reggae"));
//                genres.addGenre(QStringLiteral("Jazz"), QStringLiteral("jazz"));
//                genres.addGenre(QStringLiteral("Country"), QStringLiteral("country"));
//                genres.addGenre(QStringLiteral("Chanson"), QStringLiteral("chanson"));
//                genres.addGenre(QStringLiteral("Blues"), QStringLiteral("blues"));
//                genres.addGenre(QStringLiteral("Estrada"), QStringLiteral("estrada"));
//                genres.addGenre(QStringLiteral("Folk"), QStringLiteral("folk"));
//                genres.addGenre(QStringLiteral("Electronic"), QStringLiteral("electronic"));
//                genres.addGenre(QStringLiteral("Drum & Bass"), QStringLiteral("dnb"));
//                genres.addGenre(QStringLiteral("Soundtrack"), QStringLiteral("soundtrack"));
//                genres.addGenre(QStringLiteral("Classical"), QStringLiteral("classical"));
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
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3pm.net") % predicate; }

        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find(".mp3list .cplayer-sound-item");
                    Html::Selector author_selector(".cplayer-data-sound-author");
                    Html::Selector title_selector(".cplayer-data-sound-title");
                    Html::Selector duration_selector(".cplayer-data-sound-time");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject song_obj;

                        song_obj.insert(url_key, (*song) -> value(QStringLiteral("data-download-url")));
                        song_obj.insert(duration_key, (*song) -> find(&duration_selector).text());
                        song_obj.insert(skip_info_key, true);

                        QString author = (*song) -> find(&author_selector).text();
                        QString title = (*song) -> find(&title_selector).text();
                        title = author % QStringLiteral(" - ") % title;
                        song_obj.insert(title_key, title);

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

        QJsonArray search_postprocess(QString & predicate, bool /*by_artist*/, bool /*by_song*/, QString & /*genre*/, int /*genre_id*/, int count) {
            QString url_str = baseUrlStr(QStringLiteral("/s/f/%1/page/%2/")).arg(
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
        inline Mp3pm() : IGrabberApi() {}
        inline virtual ~Mp3pm() {}

        static Mp3pm * self;
    };
}

#endif // MP3CC
