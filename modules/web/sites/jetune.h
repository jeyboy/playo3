#ifndef JETUNE
#define JETUNE

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class Jetune : public IGrabberApi {
    public:
        inline QString name() const { return QStringLiteral("Jetune"); }
        inline Playo3::WebSubType siteType() { return Playo3::jetune_site; }

        static Jetune * instance();
        inline static void close() { delete self; }

        void genres_prepocessing() { sQuery(baseUrlStr(QStringLiteral("/genres")), genres1); }

//        QJsonArray byGenre(QString genre, const SearchLimit & limitations) { // http://zaycev.net/genres/shanson/index.html
//            QJsonArray json;
//            if (genresList().isEmpty()) genresList();

//            genre = genres.toString(genre_id);
//            if (genre.isEmpty()) return json;

//            QString url_str = baseUrlStr(QStringLiteral("/genres/%1/index_%2.html").arg(genre, page_offset_key));
//            lQuery(url_str, json, songs1, MAX_PAGE);

//            return json;
//        }

        // rus letters has specific presentation
//        QJsonArray byChar(QChar /*target_char*/, const SearchLimit & limitations) { http://zaycev.net/artist/letter-rus-zh-more.html?page=1
//            //TODO: realize later
//        }

//        // one page contains 30 albums
//        QJsonArray byType(ByTypeArg target_type, const SearchLimit & limitations) { //http://zaycev.net/musicset/more.html?page=1
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

        inline QJsonArray popular() { return sQuery(QUrl(baseUrlStr()), songs1); }

    protected:
        inline QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://www.jetune.ru") % predicate; }

        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find("img[src='/images/download.png']<tr");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject song_obj;

                        Html::Tag * link_tag = (*song) -> childTag("td", 4);

                        if (link_tag) {
                            QString link = link_tag -> childTag("a") -> link();
                            link = link.section('/', 2, 2);

                            song_obj.insert(url_key, baseUrlStr(QStringLiteral("/freedownload.php?id=") % link));
                            song_obj.insert(refresh_key, baseUrlStr(QStringLiteral("/myplayer.php?id=") % link));
                            song_obj.insert(skip_info_key, true);

                            Html::Set links = (*song) -> childTag("td", 0) -> find("a");
                            QString title = links[1] -> text() % QStringLiteral(" - ") % links[0] -> text();
                            song_obj.insert(title_key, title);

                            json << song_obj;
                        } else Logger::instance() -> writeToStream(QStringLiteral("Jetune"), QStringLiteral("Parse error"), QString(), true);
                    }

                    result = !songs.isEmpty();
                }

                case genres1: {
//                    Html::Set links = parser.find("a.genre");

//                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
//                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
//                        genres.addGenre((*link) -> text(), list[1]); // need to using alias as a part of link
//                    }
//                    result = !links.isEmpty();
                }

                default: ;
            }

            if (removeReply) delete reply;
            return result;
        }

        inline QString refresh_postprocess(QNetworkReply * /*reply*/) {
//            Html::Document parser(reply);

//            QString url = parser.find("#player_content script").text();
//            return url.section("mp3:\"", 1).section("\"", 0, 0);

            return QString();
        }

        QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
            QString url_str = baseUrlStr(
                QStringLiteral("/widesearch?ms_search_text=%1&ms_search_type=%2&ms_page=%3").arg(
                    QUrl::toPercentEncoding(predicate),
                    limitations.by_artists() ? QStringLiteral("artist") : QStringLiteral("track"),
                    page_offset_key
                )
            );

            QJsonArray json;
<<<<<<< HEAD
            lQuery(url_str, json, songs1, limitations.cpage, limitations.spage);
=======
            lQuery(url_str, json, songs1, 1/*limitations.cpage*/, limitations.spage);
>>>>>>> 0d46f1e2aec02380c5ab92f21ce44d70238504e2

            while(json.size() > limitations.count)
                json.removeLast();

            return json;
        }
    private:
        inline Jetune() : IGrabberApi() {}
        inline virtual ~Jetune() {}

        static Jetune * self;
    };
}

#endif // JETUNE
