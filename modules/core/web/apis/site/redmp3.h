#ifndef REDMP3
#define REDMP3

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class RedMp3 : public ISource, public IApi, public Singleton<RedMp3> {
        public:
            inline QString name() const { return QStringLiteral("RedMp3"); }
            inline DataSubType siteType() const { return dt_site_redmp3; }

            QJsonArray popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(), call_type_html, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://redmp3.cc") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                if (reply -> attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) return false;

                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".player");

                        QString data_title = QStringLiteral("data-title");
                        QString data_url = QStringLiteral("data-mp3url");
                        QString url_template = QStringLiteral("%1\r\nReferer: %2");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            Html::Set infos = (*track) -> find(".track-info");
                            QStringList album_and_year = infos.text().split(',');

                            track_obj.insert(tkn_grab_url, url_template.arg(baseUrlStr((*track) -> value(data_url)), baseUrlStr()));
                            track_obj.insert(tkn_grab_album, album_and_year.first());
                            track_obj.insert(tkn_grab_year, album_and_year.last());
                            track_obj.insert(tkn_grab_refresh, baseUrlStr(infos.find(".track-title").link()));
                            track_obj.insert(tkn_skip_info, true);
                            track_obj.insert(tkn_grab_title, (*track) -> value(data_title).replace(QString("—"), QString("-")));

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

            void genres_proc() {} // not supports genres


            inline QString refresh_proc(Response * /*reply*/) {
                return QString();
            }

            QJsonArray search_proc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(QStringLiteral("/mp3-%1/%2").arg(
                    encodeStr(limits.predicate.toLower().replace(QRegularExpression("[\\W_]+"), QStringLiteral("-"))),
                    OFFSET_TEMPLATE
                ));

                PolyQueryRules rules(
                    call_iter_type_page, call_iter_method_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.pages_limit, 2),
                    limits.start_page
                );
                return pRequest(url_str, call_type_html, rules, proc_tracks1);

//                QJsonArray json;
//                lQuery(url_str, json, songs1, qMin(limitations.count_page, 10), limitations.start_page - 1, limitations.total_limit); // pages at this site started from zero so we should put correction on the wind
//                return json;
            }
        private:
            friend class Singleton<RedMp3>;
            inline RedMp3() {}
            inline virtual ~RedMp3() {}
        };
    }
}

#endif // REDMP3
