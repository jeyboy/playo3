#ifndef MP3CC
#define MP3CC

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Mp3cc : public ISource, public IQueriable, public Singleton<Mp3cc> {
        public:
            inline QString name() const { return LSTR("Mp3cc"); }
            inline DataSubType sourceType() const { return dt_web_mp3cc; }

            QJsonValue popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(qst_site), call_type_html, 0, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), songs1);
            }
        protected:
            QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) {
                return LSTR("http://mp3.cc") % predicate;
            }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find(".playlist .track");
                        Html::Selector titles_selector(".playlist-name a");
                        Html::Selector down_selector("a.playlist-btn-down");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            track_obj.insert(tkn_grab_url, (*track) -> find(&down_selector).link()); // alt (*track) -> value(LSTR("data-mp3")).toString()
                            track_obj.insert(tkn_grab_duration, Duration::fromMillis((*track) -> value(LSTR("data-duration")).toInt()));
                            track_obj.insert(tkn_skip_info, true);

                            Html::Set titles = (*track) -> find(&titles_selector);
                            QString title = titles.first() -> text() % tkn_dash % titles.last() -> text();
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

            inline void genresProc() {
                genres.addGenre(LSTR("Pop"),                LSTR("pop"));
                genres.addGenre(LSTR("Rock"),               LSTR("rock"));
                genres.addGenre(LSTR("Rap"),                LSTR("rap"));
                genres.addGenre(LSTR("Alternative"),        LSTR("alternative"));
                genres.addGenre(LSTR("Rhythm and Blues"),   LSTR("rnb"));
                genres.addGenre(LSTR("Rhythm & Blues"),     LSTR("rnb"));
                genres.addGenre(LSTR("Metal"),              LSTR("metal"));
                genres.addGenre(LSTR("Reggae"),             LSTR("reggae"));
                genres.addGenre(LSTR("Jazz"),               LSTR("jazz"));
                genres.addGenre(LSTR("Country"),            LSTR("country"));
                genres.addGenre(LSTR("Chanson"),            LSTR("chanson"));
                genres.addGenre(LSTR("Blues"),              LSTR("blues"));
                genres.addGenre(LSTR("Dance"),              LSTR("dance"));
                genres.addGenre(LSTR("Estrada"),            LSTR("estrada"));
                genres.addGenre(LSTR("Easy Listening"),     LSTR("relax"));
                genres.addGenre(LSTR("Folk"),               LSTR("folk"));
                genres.addGenre(LSTR("Electronic"),         LSTR("electronic"));
                genres.addGenre(LSTR("Drum & Bass"),        LSTR("dnb"));
                genres.addGenre(LSTR("Soundtrack"),         LSTR("soundtrack"));
                genres.addGenre(LSTR("Classical"),          LSTR("classical"));
            }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site, LSTR("/search/%1/%2/page/%3/")).arg(
                    QString(limits.by_artists() ? 'a' : (limits.by_songs_name() ? 't' : 'f')),
                    encodeStr(limits.predicate),
                    OFFSET_TEMPLATE
                );

                PolyQueryRules rules(
                    call_iter_type_page, call_iter_method_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 10)
                );

                QueriableResponse response = pRequest(url_str, call_type_html, rules, 0, proc_tracks1);
                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }
        private:
            friend class Singleton<Mp3cc>;
            inline Mp3cc() {}
            inline virtual ~Mp3cc() {}
        };
    }
}

#endif // MP3CC
