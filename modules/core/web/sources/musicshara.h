#ifndef MUSICSHARA
#define MUSICSHARA

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class MusicShara : public ISource, public IQueriable, public Singleton<MusicShara> {
        public:
            inline QString name() const { return QStringLiteral("MusicShara"); }
            inline DataSubType sourceType() const { return dt_web_music_shara; }

            inline QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(qst_site), call_type_html, 0, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://musicshara.ru") % predicate; }

            QString refreshProc(Response * reply, const DataMediaType & /*itemMediaType*/) {
                Html::Document parser(reply);
                Html::Set tracks = parser.find(".options a[itemprop='audio']");
                qDebug() << "IN RESTORING" << tracks.count();

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(qst_site, tracks.link());
            }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site, QStringLiteral("/search-page-%2-%1.html?ajax=yw1")).arg(encodeStr(limits.predicate), OFFSET_TEMPLATE);

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );

                QueriableResponse response = pRequest(url_str, call_type_html, rules, 0, proc_tracks1);
                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".track");

                        Html::Selector artistSelector(".info .artist");
                        Html::Selector titleSelector(".info .title");
    //                    Html::Selector bitrateSelector(".info .bitrate");
                        Html::Selector durationSelector(".info .duration");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_url, (*track) -> data(QStringLiteral("src")));

                            QString artist = (*track) -> find(&artistSelector).text();
                            QString title = (*track) -> find(&titleSelector).text();
                            title = artist % QStringLiteral(" - ") % title;
                            track_obj.insert(tkn_grab_title, title);

                            track_obj.insert(tkn_grab_duration, (*track) -> find(&durationSelector).text());
    //                        track_obj.insert(tkn_grab_bitrate, (*track) -> find(&bitrateSelector) -> text());
                            track_obj.insert(tkn_skip_info, true);

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    default: ;
                }

                return result;
            }
        private:
            friend class Singleton<MusicShara>;
            inline MusicShara() { }

            inline virtual ~MusicShara() {}
        };
    }
}

#endif // MUSICSHARA
