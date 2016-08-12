#ifndef MP3PM
#define MP3PM

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Mp3pm : public ISource, public IQueriable, public Singleton<Mp3pm> {
        public:
            inline QString name() const { return QStringLiteral("Mp3pm"); }
            inline DataSubType sourceType() const { return dt_site_mp3pm; }

            QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(qst_site_def), call_type_html, 0, proc_tracks1);

//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3pm.net") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".mp3list .cplayer-sound-item");
                        Html::Selector author_selector(".cplayer-data-sound-author");
                        Html::Selector title_selector(".cplayer-data-sound-title");
                        Html::Selector duration_selector(".cplayer-data-sound-time");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_url, (*track) -> value(QStringLiteral("data-download-url")));
                            track_obj.insert(tkn_grab_duration, (*track) -> find(&duration_selector).text());
                            track_obj.insert(tkn_skip_info, true);

                            QString author = (*track) -> find(&author_selector).text();
                            QString title = (*track) -> find(&title_selector).text();
                            title = author % QStringLiteral(" - ") % title;
                            track_obj.insert(tkn_grab_title, title);

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

            inline bool isRefreshable() { return false; }

    //        inline void genresProc() { sQuery(baseUrlStr(QStringLiteral("/genres")), proc_genres1); }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site_def,
                    QStringLiteral("/s/f/%1/page/%2/")
                        .arg(encodeStr(limits.predicate), OFFSET_TEMPLATE)
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 10)
                );
                return pRequest(url_str, call_type_html, rules, 0, proc_tracks1);

//                QJsonArray json;
//                lQuery(url_str, json, songs1, limitations.count_page, limitations.start_page, limitations.total_limit);
//                return json;
            }
        private:
            friend class Singleton<Mp3pm>;
            inline Mp3pm() {}
            inline virtual ~Mp3pm() {}
        };
    }
}

#endif // MP3CC
