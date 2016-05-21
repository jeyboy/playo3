#ifndef MP3CC
#define MP3CC

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Mp3cc : public ISource, public IApi, public Singleton<Mp3cc> {
        public:
            inline QString name() const { return QStringLiteral("Mp3cc"); }
            inline DataSubType siteType() const { return dt_site_mp3cc; }

            QJsonArray popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(), call_type_html, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), songs1);
            }
        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) {
                return QStringLiteral("http://mp3.cc") % predicate;
            }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".playlist .track");
                        Html::Selector titles_selector(".playlist-name a");
                        Html::Selector down_selector("a.playlist-btn-down");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_url, (*track) -> find(&down_selector).link()); // alt (*track) -> value(QStringLiteral("data-mp3")).toString()
                            track_obj.insert(tkn_grab_duration, Duration::fromMillis((*track) -> value(QStringLiteral("data-duration")).toInt()));
                            track_obj.insert(tkn_skip_info, true);

                            Html::Set titles = (*track) -> find(&titles_selector);
                            QString title = titles.first() -> text() % QStringLiteral(" - ") % titles.last() -> text();
                            track_obj.insert(tkn_grab_title, title);

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

    //        inline QString refresh_proc(WebResponse * reply) {
    //            return WebManager::replyToJson(reply).value(QStringLiteral("url")).toString();
    //        }

            inline void genres_proc() {
                genres.addGenre(QStringLiteral("Pop"), QStringLiteral("pop"));
                genres.addGenre(QStringLiteral("Rock"), QStringLiteral("rock"));
                genres.addGenre(QStringLiteral("Rap"), QStringLiteral("rap"));
                genres.addGenre(QStringLiteral("Alternative"), QStringLiteral("alternative"));
                genres.addGenre(QStringLiteral("Rhythm and Blues"), QStringLiteral("rnb"));
                genres.addGenre(QStringLiteral("Rhythm & Blues"), QStringLiteral("rnb"));
                genres.addGenre(QStringLiteral("Metal"), QStringLiteral("metal"));
                genres.addGenre(QStringLiteral("Reggae"), QStringLiteral("reggae"));
                genres.addGenre(QStringLiteral("Jazz"), QStringLiteral("jazz"));
                genres.addGenre(QStringLiteral("Country"), QStringLiteral("country"));
                genres.addGenre(QStringLiteral("Chanson"), QStringLiteral("chanson"));
                genres.addGenre(QStringLiteral("Blues"), QStringLiteral("blues"));
                genres.addGenre(QStringLiteral("Dance"), QStringLiteral("dance"));
                genres.addGenre(QStringLiteral("Estrada"), QStringLiteral("estrada"));
                genres.addGenre(QStringLiteral("Easy Listening"), QStringLiteral("relax"));
                genres.addGenre(QStringLiteral("Folk"), QStringLiteral("folk"));
                genres.addGenre(QStringLiteral("Electronic"), QStringLiteral("electronic"));
                genres.addGenre(QStringLiteral("Drum & Bass"), QStringLiteral("dnb"));
                genres.addGenre(QStringLiteral("Soundtrack"), QStringLiteral("soundtrack"));
                genres.addGenre(QStringLiteral("Classical"), QStringLiteral("classical"));
            }

            QJsonArray search_proc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(QStringLiteral("/search/%1/%2/page/%3/")).arg(
                    QString(limits.by_artists() ? 'a' : (limits.by_songs() ? 't' : 'f')),
                    encodeStr(limits.predicate),
                    OFFSET_TEMPLATE
                );

                PolyQueryRules rules(
                    call_iter_type_page, call_iter_method_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.pages_limit, 10)
                );
                return pRequest(url_str, call_type_html, rules, proc_tracks1);

//                QJsonArray json;
//                lQuery(url_str, json, songs1, limitations.count_page, limitations.start_page, limitations.total_limit);
//                return json;
            }
        private:
            friend class Singleton<Mp3cc>;
            inline Mp3cc() {}
            inline virtual ~Mp3cc() {}
        };
    }
}

#endif // MP3CC
