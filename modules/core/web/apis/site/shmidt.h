#ifndef SHMIDT
#define SHMIDT

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Shmidt : public ISource, public IApi, public Singleton<Shmidt> {
        public:
            inline QString name() const { return QStringLiteral("Shmidt"); }
            inline DataSubType siteType() const { return dt_site_shmidt; }

            QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(), call_type_html, 0, proc_tracks1);

//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3.shmidt.net") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".artist ul a");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_refresh, baseUrlStr((*track) -> link()));
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

    //        inline void genresProc() { sQuery(baseUrlStr(QStringLiteral("/music")), genres1); }

            inline QString refreshProc(Response * reply, const DataMediaType & /*itemMediaType*/) {
                Html::Document parser(reply);
                Html::Set tracks = parser.find(".download p a");

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(tracks.link());
            }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(QStringLiteral("/search.php?userquery=%1&type=%2")).arg(
                    encodeStr(limits.predicate),
                    (limits.by_artists() ? QStringLiteral("artistname") : limits.by_songs_name() ? QStringLiteral("songtitle") : QStringLiteral("entire"))
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );
                return pRequest(url_str, call_type_html, rules, 0, proc_tracks1);

//                QJsonArray json;
//                toJson(songs1, Manager::prepare() -> followedForm(QUrl(url_str)), json, true);
//                return json;
            }
        private:
            friend class Singleton<Shmidt>;
            inline Shmidt() {}
            inline virtual ~Shmidt() {}
        };
    }
}

#endif // SHMIDT
