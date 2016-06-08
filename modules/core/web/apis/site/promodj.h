#ifndef PROMODJ
#define PROMODJ

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

#define ITEMS_PER_PAGE 50

// store all selectors in global variables
namespace Core {
    namespace Web {
        class PromoDj : public ISource, public IApi, public Singleton<PromoDj> {
        public:
            inline QString name() const { return QStringLiteral("PromoDJ"); }
            inline DataSubType siteType() const { return dt_site_promodj; }

            QJsonArray popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(), call_type_html, 0, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://promodj.com") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find("#content .track2");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            Html::Tag * title = (*track) -> find(".title a").first();
                            QString link = (*track) -> find(".downloads_count a").link();
                            if (link.isEmpty()) {
                                link = title -> link();
                                link = link.section('/', 0, 2) % QStringLiteral("/prelisten/") % link.section('/', 5);
                            }

                            track_obj.insert(tkn_grab_url, link);
                            track_obj.insert(tkn_grab_title, title -> text());
                            track_obj.insert(tkn_skip_info, true);

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    case proc_genres1: {
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

                return result;
            }

            inline void genresProc() {
                sRequest(baseUrlStr(QStringLiteral("/music")), call_type_html, 0, proc_genres1);
//                sQuery(baseUrlStr(QStringLiteral("/music")), genres1);
            }

    //        http://promodj.com/prelisten/5338563/Beck_Sarbassov_DJ_Zhasulan_Baikenov_Time_flies.mp3
    //        inline QString refreshProc(WebResponse * reply) {
    //            Html::Document parser(reply);

    //            QString url = parser.find("#flash_prelisten script").text();
    //            return url.section("URL\":\"", 1).section("\"", 0, 0);
    //        }

//            inline bool isRefreshable() { return false; }

            QJsonArray searchProc(const SearchLimit & limits) {
                // alt search http://promodj.com/search?searchfor=lol&mode=audio&sortby=relevance&period=all

                QString alias = genresList().toAlias(limits.genre);
                QString url_str = baseUrlStr(QStringLiteral("/music%1?kind=music&styleID=&searchfor=%2&page=%3")).arg(
                            (alias.isEmpty() ? QString() : QStringLiteral("/")) % alias, encodeStr(limits.predicate), OFFSET_TEMPLATE);

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 10)
                );

                return pRequest(url_str, call_type_html, rules, proc_tracks1);

//                QJsonArray json;
//                lQuery(url_str, json, proc_tracks1, qMin(limits.pages_limit, 10), limits.start_page, limits.items_limit);

//                return json;
            }
        private:
            friend class Singleton<PromoDj>;
            inline PromoDj() {}
            inline virtual ~PromoDj() {}
        };
    }
}

#endif // PROMODJ
