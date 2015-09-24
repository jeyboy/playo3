#ifndef MP3BASE
#define MP3BASE

#include "modules/core/interfaces/igrabber_api.h"

//function playSong(id)
//{
//	url = '/user/player/?song='+id;
//	openPlayer(url);
//}

//function playAlbum(id)
//{
//	url = '/user/player/?album='+id;
//	openPlayer(url);
//}

//function playFavorites(id)
//{
//	url = '/user/player/?favorites';
//	openPlayer(url);
//}

//function playTopSongs()
//{
//	url = '/user/player/?top_songs';
//	openPlayer(url);
//}

//function playTag(id)
//{
//	url = '/user/player/?tag='+id;
//	openPlayer(url);
//}

//function playArtist(id)
//{
//	url = '/user/player/?artist='+id;
//	openPlayer(url);
//}



//function downloadSong(id)
//{
//	url = '/user/download/?song='+id;
//	openURL(url);
//}

//function downloadSongClient(id)
//{
//	url = '/user/download/?client&song='+id;
//	openURL(url);
//}

//function downloadAlbum(id)
//{
//	url = '/user/download/?album='+id;
//	openURL(url);
//}

//function downloadAlbumClient(id)
//{
//	url = '/user/download/?client&album='+id;
//	openURL(url);
//}

//function downloadFavorites(id)
//{
//	url = '/user/download/?favorites';
//	openURL(url);
//}

//function downloadTopSongs()
//{
//	url = '/user/download/?top_songs';
//	openURL(url);
//}

//function downloadTag(id)
//{
//	url = '/user/download/?tag='+id;
//	openURL(url);
//}

//function downloadArtist(id)
//{
//	url = '/user/download/?artist='+id;
//	openURL(url);
//}

//function downloadArtistClient(id)
//{
//	url = '/user/download/?client&artist='+id;
//	openURL(url);
//}

// store all selectors in global variables
namespace Grabber {
    class Mp3Base : public IGrabberApi {
    public:
        static Mp3Base * instance();
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Mp3Base"); }
        inline Playo3::WebSubType siteType() { return Playo3::mp3base_site; }

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
        QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3base.cc") % predicate; }

        bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);
            bool result = false;

            switch(jtype) {
                case songs1: {
                    Html::Set songs = parser.find("table.files tr");

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject song_obj;

                        QString link = (*song) -> find(".download_icon").link();
                        if (link.isEmpty()) continue;

                        link = link.section('(', 1).section(')', 0, 0);

                        song_obj.insert(refresh_key, baseUrlStr(QStringLiteral("/user/player/?song=") % link));
                        song_obj.insert(duration_key, (*song) -> childTag("td", 5) -> toText());
                        song_obj.insert(skip_info_key, true);

                        QString artist = (*song) -> childTag(QStringLiteral("td"), 2) -> toText();
                        QString title = (*song) -> childTag(QStringLiteral("td"), 1) -> toText();
                        title = artist % QStringLiteral(" - ") % title;
                        song_obj.insert(title_key, title);

                        json << song_obj;
                    }

                    result = !songs.isEmpty();
                }

                case genres1: {
                    Html::Set links = parser.find("a.genre");

                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                        genres.addGenre((*link) -> text(), list[1]); // need to using alias as a part of link
                    }
                    result = !links.isEmpty();
                }

                default: ;
            }

            if (removeReply) delete reply;
            return result;
        }

        inline void genres_prepocessing() { sQuery(baseUrlStr(QStringLiteral("/genres")), genres1); }

        inline QString refresh_postprocess(WebResponse * reply) {
            Html::Document parser(reply);

            QString url = parser.find("#player_content script").text();
            return url.section("mp3:\"", 1).section("\"", 0, 0);
        }

        QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
            QString url_str = baseUrlStr(QStringLiteral("/search?q=%1&page=%2")).arg(encodeStr(predicate), page_offset_key);

            QJsonArray json;
            lQuery(url_str, json, songs1, limitations.cpage, limitations.spage);

            while(json.size() > limitations.count)
                json.removeLast();

            return json;
        }
    private:
        inline Mp3Base() : IGrabberApi() {}
        inline virtual ~Mp3Base() {}

        static Mp3Base * self;
    };
}

#endif // MP3BASE
