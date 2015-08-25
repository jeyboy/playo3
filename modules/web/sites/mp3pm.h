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

        inline QString name() const { return QStringLiteral("Mp3pm"); }
        inline Playo3::WebSubType siteType() { return Playo3::mp3pm_site; }

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

        QJsonArray popular() { return sQuery(QUrl(baseUrlStr()), songs1); }

    protected:
        QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3pm.net") % predicate; }

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

//        inline void genres_prepocessing() { sQuery(baseUrlStr(QStringLiteral("/genres")), genres1); }

        QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
            QString url_str = baseUrlStr(QStringLiteral("/s/f/%1/page/%2/")).arg(
                encodeStr(predicate),
                page_offset_key
            );

            QJsonArray json;
            lQuery(url_str, json, songs1, limitations.cpage, limitations.spage);

            while(json.size() > limitations.count)
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
