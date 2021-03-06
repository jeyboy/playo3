#ifndef JETUNE
#define JETUNE

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Jetune : public ISource, public IQueriable, public Singleton<Jetune> {
        public:
            inline QString name() const { return LSTR("Jetune"); }
            inline DataSubType sourceType() const { return dt_web_jetune; }

            inline QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(qst_site), call_type_html, 0, proc_tracks1);
            }

        protected:
            inline QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) { return LSTR("http://www.jetune.ru") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & message, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find("img[src='/images/download.png']<tr");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            Html::Tag * link_tag = (*track) -> childTag("td", 4);

                            if (link_tag) {
                                QString link = link_tag -> childTag("a") -> link();
                                link = link.section('/', 2, 2);

                                track_obj.insert(tkn_grab_url, baseUrlStr(qst_site, LSTR("/freedownload.php?id=") % link));
                                track_obj.insert(tkn_grab_refresh, baseUrlStr(qst_site, LSTR("/myplayer.php?id=") % link));
                                track_obj.insert(tkn_skip_info, true);

                                Html::Set links = (*track) -> childTag("td", 0) -> find("a");
                                QString title = links[1] -> text() % tkn_dash % links[0] -> text();
                                track_obj.insert(tkn_grab_title, title);

                                arg -> append(track_obj, track + 1 == tracks.end());
                            }
                            else message = LSTR("Jetune: Parse error");
                        }

                        result = !tracks.isEmpty();
                    }

                    case proc_genres1: {
    //                    Html::Set links = parser.find("a.genre");

    //                    for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
    //                        QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
    //                        genres.addGenre((*link) -> text(), list[1]); // need to using alias as a part of link
    //                    }
    //                    result = !links.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

            inline void genresProc() {
                sRequest(baseUrlStr(qst_site, LSTR("/genres")), call_type_html, 0, proc_genres1);

//                sQuery(baseUrlStr(LSTR("/genres")), genres1);
            }

            inline bool isRefreshable() { return false; }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site,
                    QStringLiteral("/widesearch?ms_search_text=%1&ms_search_type=%2&ms_page=%3").arg(
                        encodeStr(limits.predicate),
                        limits.by_artists() ? LSTR("artist") : LSTR("track"),
                        OFFSET_TEMPLATE
                    )
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset.toInt(),
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 1)
                );

                QueriableResponse response = pRequest(url_str, call_type_html, rules, 0, proc_tracks1);
                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }
        private:
            friend class Singleton<Jetune>;
            inline Jetune() {}
            inline virtual ~Jetune() {}
        };
    }
}

#endif // JETUNE
