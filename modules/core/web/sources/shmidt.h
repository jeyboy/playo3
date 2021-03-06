#ifndef SHMIDT
#define SHMIDT

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Shmidt : public ISource, public IQueriable, public Singleton<Shmidt> {
        public:
            inline QString name() const { return LSTR("Shmidt"); }
            inline DataSubType sourceType() const { return dt_web_shmidt; }

            QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(qst_site), call_type_html, 0, proc_tracks1);

//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) { return LSTR("http://mp3.shmidt.net") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".artist ul a");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_refresh, baseUrlStr(qst_site, (*track) -> link()));
                            track_obj.insert(tkn_skip_info, true);
                            track_obj.insert(tkn_grab_title, (*track) -> text());

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

    //        inline void genresProc() { sQuery(baseUrlStr(LSTR("/music")), genres1); }

            inline QString refreshProc(Response * reply, const DataMediaType & /*itemMediaType*/) {
                Html::Document parser(reply);
                Html::Set tracks = parser.find(".download p a");

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(qst_site, tracks.link());
            }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site, LSTR("/search.php?userquery=%1&type=%2")).arg(
                    encodeStr(limits.predicate),
                    (limits.by_artists() ? LSTR("artistname") : limits.by_songs_name() ? LSTR("songtitle") : LSTR("entire"))
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset.toInt(),
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );

                QueriableResponse response = pRequest(url_str, call_type_html, rules, 0, proc_tracks1);
                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }
        private:
            friend class Singleton<Shmidt>;
            inline Shmidt() {}
            inline virtual ~Shmidt() {}
        };
    }
}

#endif // SHMIDT
