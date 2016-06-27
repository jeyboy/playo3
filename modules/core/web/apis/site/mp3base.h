#ifndef MP3BASE
#define MP3BASE

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

//function playSong(id)
//{
//	url = '/user/player/?song='+id;
//	openPlayer(url);
//}

//function playAlbum(id)
//{
//	url = '/user/player/?album='+id;
//	openPlayer(url);
//}

//function playFavorites(id)
//{
//	url = '/user/player/?favorites';
//	openPlayer(url);
//}

//function playTopSongs()
//{
//	url = '/user/player/?top_songs';
//	openPlayer(url);
//}

//function playTag(id)
//{
//	url = '/user/player/?tag='+id;
//	openPlayer(url);
//}

//function playArtist(id)
//{
//	url = '/user/player/?artist='+id;
//	openPlayer(url);
//}



//function downloadSong(id)
//{
//	url = '/user/download/?song='+id;
//	openURL(url);
//}

//function downloadSongClient(id)
//{
//	url = '/user/download/?client&song='+id;
//	openURL(url);
//}

//function downloadAlbum(id)
//{
//	url = '/user/download/?album='+id;
//	openURL(url);
//}

//function downloadAlbumClient(id)
//{
//	url = '/user/download/?client&album='+id;
//	openURL(url);
//}

//function downloadFavorites(id)
//{
//	url = '/user/download/?favorites';
//	openURL(url);
//}

//function downloadTopSongs()
//{
//	url = '/user/download/?top_songs';
//	openURL(url);
//}

//function downloadTag(id)
//{
//	url = '/user/download/?tag='+id;
//	openURL(url);
//}

//function downloadArtist(id)
//{
//	url = '/user/download/?artist='+id;
//	openURL(url);
//}

//function downloadArtistClient(id)
//{
//	url = '/user/download/?client&artist='+id;
//	openURL(url);
//}

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Mp3Base : public ISource, public IQueriable, public Singleton<Mp3Base> {
        public:
            inline QString name() const { return QStringLiteral("Mp3Base"); }
            inline DataSubType siteType() const { return dt_site_mp3base; }

            QJsonValue popular(const SearchLimit & /*limitations*/) {
                return saRequest(baseUrlStr(), call_type_html, 0, proc_tracks1);

//                return sQuery(QUrl(baseUrlStr()), songs1);
            }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://mp3base.cc") % predicate; }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
                        Html::Set tracks = parser.find("table.files tr");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            QString link = (*track) -> find(".play_icon").link();
                            if (link.isEmpty()) continue;

                            link = link.section('(', 1).section(')', 0, 0);

                            track_obj.insert(tkn_grab_refresh, baseUrlStr(QStringLiteral("/user/player/?song=") % link));

                            QString artist = (*track) -> childTag(QStringLiteral("td"), 2) -> toText();
                            QString title = (*track) -> childTag(QStringLiteral("td"), 1) -> toText();
                            title = artist % QStringLiteral(" - ") % title;
                            track_obj.insert(tkn_grab_title, title);

                            Html::Tag * duration_tag = (*track) -> childTag("td", 5);
                            if (duration_tag)
                                track_obj.insert(tkn_grab_duration, duration_tag -> toText());
                            else
                                qDebug() << title << "DID NOT HAVE DURATION";
                            track_obj.insert(tkn_skip_info, true);

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    case proc_genres1: {
                        Html::Set links = parser.find("a.genre");

                        for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                            QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                            genres.addGenre((*link) -> text(), list[1]); // need to using alias as a part of link
                        }
                        result = !links.isEmpty();
                    }

                    default: ;
                }

                return result;
            }

            inline void genresProc() {
                sRequest(baseUrlStr(QStringLiteral("/genres")), call_type_html, 0, proc_genres1);
//                sQuery(baseUrlStr(QStringLiteral("/genres")), genres1);
            }

            inline QString refreshProc(Response * reply, const DataMediaType & /*itemMediaType*/) {
                Html::Document parser(reply);

                QString url = parser.find("#player_content script").text();
                return url.section("mp3:\"", 1).section("\"", 0, 0);
            }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(
                    QStringLiteral("/search?q=%1&page=%2")
                        .arg(encodeStr(limits.predicate), OFFSET_TEMPLATE)
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), limits.requests_limit
                );

                return pRequest(url_str, call_type_html, rules, 0, proc_tracks1);

//                QJsonArray json;
//                lQuery(url_str, json, proc_songs1, limits.count_page, limits.start_page, limits.items_limit);

//                return json;
            }
        private:
            friend class Singleton<Mp3Base>;
            inline Mp3Base() {}
            inline virtual ~Mp3Base() {}
        };
    }
}

#endif // MP3BASE
