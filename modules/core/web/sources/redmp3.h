#ifndef REDMP3
#define REDMP3

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class RedMp3 : public ISource, public IQueriable, public Singleton<RedMp3> {
        public:
            inline QString name() const { return QStringLiteral("RedMp3"); }
            inline DataSubType sourceType() const { return dt_web_redmp3; }

            QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(qst_site), call_type_html, 0, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://redmp3.cc") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
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

                            track_obj.insert(tkn_grab_url, url_template.arg(baseUrlStr(qst_site, (*track) -> value(data_url)), baseUrlStr(qst_site)));
                            track_obj.insert(tkn_grab_album, album_and_year.first());
                            track_obj.insert(tkn_grab_year, album_and_year.last());
                            track_obj.insert(tkn_grab_refresh, baseUrlStr(qst_site, infos.find(".track-title").link()));
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

            void genresProc() {} // not supports genres

            inline bool isRefreshable() { return false; }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site, QStringLiteral("/mp3-%1/%2").arg(
                    encodeStr(limits.predicate.toLower().replace(QRegularExpression("[\\W_]+"), QStringLiteral("-"))),
                    OFFSET_TEMPLATE
                ));

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );

                QueriableResponse response = pRequest(url_str, call_type_html, rules, 0, proc_tracks1);
                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }
        private:
            friend class Singleton<RedMp3>;
            inline RedMp3() {}
            inline virtual ~RedMp3() {}
        };
    }
}

#endif // REDMP3
