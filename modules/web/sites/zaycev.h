#ifndef ZAYCEV
#define ZAYCEV

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 47
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class Zaycev : public IGrabberApi {
    public:
        static Zaycev * instance();
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Zaycev"); }
        inline Playo3::WebSubType siteType() { return Playo3::zaycev_site; }

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

//        QJsonArray byGenre(QString genre, bool is_popular) { // http://zaycev.net/genres/shanson/index.html
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
        QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://zaycev.net") % predicate; }


        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find(".musicset-track-list__items .musicset-track"); //.track-is-banned // banned track is not playable for some countries

                    QString ban_class = QStringLiteral("track-is-banned");
                    QString data_url = QStringLiteral("data-url");

                    Html::Selector artist_selector(".musicset-track__artist a");
                    Html::Selector title_selector(".musicset-track__track-name a");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        if ((*song) -> hasClass(ban_class)) continue;

                        QJsonObject song_obj;

                        song_obj.insert(refresh_key, baseUrlStr((*song) -> value(data_url)));
                        song_obj.insert(duration_key, Duration::fromSeconds((*song) -> value(QStringLiteral("data-duration")).toInt()));
                        song_obj.insert(skip_info_key, true);

                        QString artist = (*song) -> find(&artist_selector).text();
                        QString title = (*song) -> find(&title_selector).text();
                        title = artist % QStringLiteral(" - ") % title;
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

        // {"url":"http://dl.zaycev.net/85673/2745662/rick_ross_-_love_sosa.mp3?dlKind=play&format=json"}
        inline QString refresh_postprocess(QNetworkReply * reply) {
            return WebManager::replyToJson(reply).value(QStringLiteral("url")).toString();
        }

        QJsonArray search_postprocess(QString & predicate, PredicateType /*ptype*/, QString & /*genre*/, bool /*popular*/, int count) {
            QString url_str = baseUrlStr(QStringLiteral("/search.html?query_search=%1&page=%2").arg(
                QUrl::toPercentEncoding(predicate),
                page_offset_key
            ));

            QJsonArray json;

            lQuery(url_str, json, songs1, 2/*MAX_PAGE*/); // 2 pages at this time // if pagination overlimited - 302 status received

            while(json.size() > count)
                json.removeLast();

            return json;
        }
    private:
        inline Zaycev() : IGrabberApi() {}
        inline virtual ~Zaycev() {}

        static Zaycev * self;
    };
}

#endif // ZAYCEV
