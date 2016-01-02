#ifndef REDMP3
#define REDMP3

#include "modules/core/web/interfaces/igrabber_api.h"
#include "modules/core/interfaces/singleton.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        using namespace Grabber;
        class RedMp3 : public IGrabberApi, public Singleton<RedMp3> {
        public:
            inline QString name() const { return QStringLiteral("RedMp3"); }
            inline SubType siteType() { return site_redmp3; }

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
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://redmp3.cc") % predicate; }

            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                if (reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) return false;

                Html::Document parser(reply);
                bool result = false;

                switch(jtype) {
                    case songs1: {
                        Html::Set songs = parser.find(".player");

                        QString data_title = QStringLiteral("data-title");
                        QString data_url = QStringLiteral("data-mp3url");
                        QString url_template = QStringLiteral("%1\r\nReferer: %2");

                        for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                            QJsonObject song_obj;

                            Html::Set infos = (*song) -> find(".track-info");
                            QStringList album_and_year = infos.text().split(',');

                            song_obj.insert(url_key, url_template.arg(baseUrlStr((*song) -> value(data_url)), baseUrlStr(), DEFAULT_AGENT));
                            song_obj.insert(album_key, album_and_year.first());
                            song_obj.insert(year_key, album_and_year.last());
                            song_obj.insert(refresh_key, baseUrlStr(infos.find(".track-title").link()));
//                            song_obj.insert(skip_info_key, true);
                            song_obj.insert(title_key, (*song) -> value(data_title));
                            json << song_obj;
                        }

                        result = !songs.isEmpty();
                    }

                    default: ;
                }

                if (removeReply) delete reply;
                return result;
            }

            void genres_prepocessing() {} // not supports genres


            inline QString refresh_postprocess(Response * reply) {
//                return reply -> toJson().value(QStringLiteral("url")).toString();
            }

            QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                QString url_str = baseUrlStr(QStringLiteral("/mp3-%1/%2").arg(
                    encodeStr(predicate.toLower().replace(QRegularExpression("[\W_]+"), QStringLiteral("-"))),
                    page_offset_key
                ));

                QJsonArray json;

                lQuery(url_str, json, songs1, qMin(limitations.count_page, 10), limitations.start_page - 1, limitations.total_limit); // pages at this site started from zero so we should put correction on the wind

//                while(json.size() > limitations.count)
//                    json.removeLast();

                return json;
            }
        private:
            friend class Singleton<RedMp3>;
            inline RedMp3() {}
            inline virtual ~RedMp3() {}
        };
    }
}

#endif // REDMP3
