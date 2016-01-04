#ifndef SHMIDT
#define SHMIDT

#include "modules/core/web/interfaces/igrabber_api.h"
#include "modules/core/interfaces/singleton.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        using namespace Grabber;

        class Shmidt : public IGrabberApi, public Singleton<Shmidt> {
        public:
            inline QString name() const { return QStringLiteral("Shmidt"); }
            inline SubType siteType() { return site_shmidt; }

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

            QJsonArray popular(QString & /*genre*/) { return sQuery(QUrl(baseUrlStr()), songs1); }

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

    //        inline void genres_prepocessing() { sQuery(baseUrlStr(QStringLiteral("/music")), genres1); }

            inline QString refresh_postprocess(Response * reply) {
                Html::Document parser(reply);
                Html::Set tracks = parser.find(".download p a");

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(tracks.link());
            }

            QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                QString url_str = baseUrlStr(QStringLiteral("/search.php?userquery=%1&type=%2")).arg(
                    encodeStr(predicate),
                    (limitations.by_artists() ? QStringLiteral("artistname") : limitations.by_songs() ? QStringLiteral("songtitle") : QStringLiteral("entire"))
                );

                QJsonArray json;
                toJson(songs1, Manager::prepare() -> followedForm(QUrl(url_str)), json, true);

//                while(json.size() > limitations.total_limit)
//                    json.removeLast();

                return json;
            }
        private:
            friend class Singleton<Shmidt>;
            inline Shmidt() {}
            inline virtual ~Shmidt() {}
        };
    }
}

#endif // SHMIDT
