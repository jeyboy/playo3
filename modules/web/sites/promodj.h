#ifndef PROMODJ
#define PROMODJ

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Grabber {
    class PromoDj : public IGrabberApi {
    public:
        static PromoDj * instance();
        inline static void close() { delete self; }

        TargetGenres genresList() {
            if (genres.isEmpty())
                sQuery(baseUrlStr(QStringLiteral("/music")), genres1);

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
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://promodj.com") % predicate; }

        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find("#content .track2");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject song_obj;

                        Html::Tag * title = (*song) -> find(".title a").first();
                        QString link = (*song) -> find(".downloads_count a").link();
                        if (link.isEmpty()) {
                            link = title -> link();
                            link = link.section('/', 0, 2) % QStringLiteral("/prelisten/") % link.section('/', 5);
                        }

                        song_obj.insert(url_key, link);
                        song_obj.insert(title_key, title -> text());
                        song_obj.insert(skip_info_key, true);

                        json << song_obj;
                    }

                    result = !songs.isEmpty();
                }

                case genres1: {
                    Html::Set links = parser.find(".styles_tagcloud a");

                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                        if (list.length() > 1)
                            genres.addGenre((*link) -> text(), list[1]);
                    }
                    result = !links.isEmpty();
                }

                default: ;
            }

            if (removeReply) delete reply;
            return result;
        }

//        http://promodj.com/prelisten/5338563/Beck_Sarbassov_DJ_Zhasulan_Baikenov_Time_flies.mp3
//        inline QString refresh_postprocess(QNetworkReply * reply) {
//            Html::Document parser(reply);

//            QString url = parser.find("#flash_prelisten script").text();
//            return url.section("URL\":\"", 1).section("\"", 0, 0);
//        }

        QJsonArray search_postprocess(QString & predicate, bool /*by_artist*/, bool /*by_song*/, QString & genre, int /*genre_id*/, int count) {
            // alt search http://promodj.com/search?searchfor=lol&mode=audio&sortby=relevance&period=all

            QString alias = genresList().toAlias(genre);
            QString url_str = baseUrlStr(QStringLiteral("/music%1?kind=music&styleID=&searchfor=%2&page=%3")).arg(
                        (alias.isEmpty() ? QString() : QStringLiteral("/")) % alias, QUrl::toPercentEncoding(predicate), page_offset_key);

            QJsonArray json;
            lQuery(url_str, json, songs1, 10); // ten page at this time

            while(json.size() > count)
                json.removeLast();

            return json;
        }
    private:
        inline PromoDj() : IGrabberApi() {}
        inline virtual ~PromoDj() {}

        static PromoDj * self;
    };
}

#endif // PROMODJ
