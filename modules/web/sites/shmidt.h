#ifndef SHMIDT
#define SHMIDT

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 47
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class Shmidt : public IGrabberApi {
    public:
        static Shmidt * instance();
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Shmidt"); }
        inline Playo3::WebSubType siteType() { return Playo3::shmidt_site; }

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
        QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3.shmidt.net") % predicate; }

        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find(".artist ul a");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject song_obj;

                        song_obj.insert(refresh_key, baseUrlStr((*song) -> link()));
                        song_obj.insert(skip_info_key, true);
                        song_obj.insert(title_key, (*song) -> text());

                        json << song_obj;
                    }

                    result = !songs.isEmpty();
                }

                default: ;
            }

            if (removeReply) delete reply;
            return result;
        }

        inline QString refresh_postprocess(QNetworkReply * reply) {
            Html::Document parser(reply);
            Html::Set tracks = parser.find(".download p a");

            if (tracks.isEmpty())
                return QString();
            else
                return baseUrlStr(tracks.link());
        }

        QJsonArray search_postprocess(QString & predicate, bool by_artist, bool by_song, QString & /*genre*/, int /*genre_id*/, int count) {
            QString url_str = baseUrlStr(QStringLiteral("/search.php?userquery=%1&type=%2")).arg(
                QUrl::toPercentEncoding(predicate),
                (by_artist ? QStringLiteral("entire") : by_song ? QStringLiteral("songtitle") : QStringLiteral("artistname"))
            );

            QJsonArray json;
            toJson(songs1, WebManager::manager() -> postForm(QUrl(url_str)), json, true);

            while(json.size() > count)
                json.removeLast();

            return json;
        }
    private:
        inline Shmidt() : IGrabberApi() {}
        inline virtual ~Shmidt() {}

        static Shmidt * self;
    };
}

#endif // SHMIDT
