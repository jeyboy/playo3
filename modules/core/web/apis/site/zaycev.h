#ifndef ZAYCEV
#define ZAYCEV

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Zaycev : public ISource, public IApi, public Singleton<Zaycev> {
        public:
            inline QString name() const { return QStringLiteral("Zaycev"); }
            inline DataSubType siteType() const { return dt_site_zaycev; }

    //        QJsonArray searchByGenre(const SearchLimit & /*limitations*/) { // http://zaycev.net/genres/shanson/index.html
    //            QJsonArray json;
    //            if (genresList().isEmpty()) genresList();

    //            genre = genres.toString(genre_id);
    //            if (genre.isEmpty()) return json;

    //            QString url_str = baseUrlStr(QStringLiteral("/genres/%1/index_%2.html").arg(genre, page_offset_key));
    //            lQuery(url_str, json, songs1, MAX_PAGE);

    //            return json;
    //        }

            // rus letters has specific presentation
    //        QJsonArray searchByChar(const SearchLimit & /*limitations*/) { http://zaycev.net/artist/letter-rus-zh-more.html?page=1
    //            //TODO: realize later
    //        }

    //        // one page contains 30 albums
    //        QJsonArray searchByType(const SearchLimit & /*limitations*/) { //http://zaycev.net/musicset/more.html?page=1
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

            QJsonArray popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(), call_type_html, 0, proc_tracks1);

//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://zaycev.net") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                if (reply -> status() != 200) return false; // if pagination overlimited - 302 status received

                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".musicset-track-list__items .musicset-track"); //.track-is-banned // banned track is not playable for some countries

                        QString ban_class = QStringLiteral("track-is-banned");
                        QString data_url = QStringLiteral("data-url");

                        Html::Selector artist_selector(".musicset-track__artist a");
                        Html::Selector title_selector(".musicset-track__track-name a");

                        qDebug() << "TRACKS AMOUNT" << tracks.size();
                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            qDebug() << "TRACKS" << (*track) -> hasClass(ban_class);
                            if ((*track) -> hasClass(ban_class)) continue;

                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_refresh, baseUrlStr((*track) -> value(data_url)));
                            track_obj.insert(tkn_grab_duration, Duration::fromSeconds((*track) -> value(QStringLiteral("data-duration")).toInt()));
                            track_obj.insert(tkn_skip_info, true);

                            QString artist = (*track) -> find(&artist_selector).text();
                            QString title = (*track) -> find(&title_selector).text();
                            title = artist % QStringLiteral(" - ") % title;
                            track_obj.insert(tkn_grab_title, title);

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

            void genres_proc() { // manual init at this time
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

            // {"url":"http://dl.zaycev.net/85673/2745662/rick_ross_-_love_sosa.mp3?dlKind=play&format=json"}
            inline QString refresh_proc(Response * reply) {
                return reply -> toJson().value(QStringLiteral("url")).toString();
            }

            QJsonArray search_proc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(
                    QStringLiteral("/search.html?query_search=%1&page=%2")
                        .arg(encodeStr(limits.predicate), OFFSET_TEMPLATE)
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );
                return pRequest(url_str, call_type_html, rules, proc_tracks1);


//                QJsonArray json;
//                lQuery(url_str, json, proc_tracks1, qMin(limitations.count_page, 2), limitations.start_page, limitations.total_limit); // 2 pages at this time
//                return json;
            }
        private:
            friend class Singleton<Zaycev>;
            inline Zaycev() {}
            inline virtual ~Zaycev() {}
        };
    }
}

#endif // ZAYCEV
