#ifndef OD_REQUEST_API
#define OD_REQUEST_API

#include "od_api_misc.h"
#include "modules/core/web/interfaces/teu_auth.h"

#define OD_OFFSET_LIMIT 2000
#define OD_SEARCH_LIMIT 200

namespace Core {
    namespace Web {
        namespace Od {
            class RequestApi : public Misc, public TeuAuth {
            protected:
                RequestApi() {}

                bool sessionIsValid() { return !hasError(Manager::prepare() -> getJson(initAudioUrl())); }

                inline QUrl authRequestUrl(const QString & email, const QString & pass) const {
                    return QUrl(base_auth_url % "https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=" + encodeStr(email) + "&st.password=" + encodeStr(pass) + "&st.remember=on&st.iscode=false");
                }

                inline QUrl initUrl() const { return QUrl(base_auth_url % "dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=" % additional()); }

                inline QUrl audioUrl(const QString func, const QUrlQuery & query = QUrlQuery()) {
                    QUrl url(base_audio_url % func % QStringLiteral(";") % genDefaultParams().toString());
                    url.setQuery(query);
                    return url;
                }

                inline QUrl addPlaylistUrl() { return audioUrl(QStringLiteral("playlistsAdd")); } // params : (name: '') (publicPlaylist: 'true')
                inline QUrl removePlaylistUrl() { return audioUrl(QStringLiteral("playlistsRemove")); } // params : (pid: '' (id of playlist))
                inline QUrl getPlaylistsUrl() { return audioUrl(QStringLiteral("playlistsGet")); } // params: maybe has uid param ? need to check
                inline QUrl addToPlaylistsUrl() { return audioUrl(QStringLiteral("like")); } // params: (pid: playlist id) (pos: pos in playlist ?) (type: 'formaddpl' ?) (tid: id of track) and pagination attrs ?
                inline QUrl removeFromPlaylistsUrl() { return audioUrl(QStringLiteral("dislike")); } // params: (pid: playlist id) (tid: id of track) and pagination attrs ? (start: '0' count: '1')

                inline QUrl searchUrl(const QString & predicate) { return audioUrl(QStringLiteral("relevant"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs
                inline QUrl searchTracksUrl(const QString & predicate) { return audioUrl(QStringLiteral("tracks"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs
                inline QUrl searchAlbumsUrl(const QString & predicate) { return audioUrl(QStringLiteral("albums"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs
                inline QUrl searchArtistsUrl(const QString & predicate) { return audioUrl(QStringLiteral("artists"), QUrlQuery(QStringLiteral("q=%1").arg(predicate))); } // params : (q: predicate) and pagination attrs

                inline QUrl albumAudiosUrl() { return audioUrl(QStringLiteral("album")); } // params : (albumId: album id)
                inline QUrl artistAudiosUrl() { return audioUrl(QStringLiteral("artist")); } // params : (artistId: artist id)


                inline QUrl collectionsAudioUrl() { return audioUrl(QStringLiteral("collections")); } // params: (collectionId: not used for index of collections) and pagination attrs
                inline QUrl downloadedAudioUrl() { return audioUrl(QStringLiteral("downloaded")); } // params : pagination attrs
                inline QUrl isDownloadedAudioUrl() { return audioUrl(QStringLiteral("isDownloaded")); } // params : (tid: track id)
                inline QUrl listenedAudioUrl() { return audioUrl(QStringLiteral("history")); } // params : pagination attrs
                inline QUrl popularAudioUrl() { return audioUrl(QStringLiteral("popTracks")); }

                // artists // tracks // tuners // collections // albums
                inline QUrl popAudioUrl() { return audioUrl(QStringLiteral("pop")); } // (locale: 'ru') and pagination attrs
                inline QUrl customAudioUrl() { return audioUrl(QStringLiteral("custom")); } // param (ids: ids of (track / album / artist) splited by coma) ? // info request per ids for items
        //        inline QUrl formaddplUrl() { return audioUrl(QStringLiteral("formaddpl")); } // params: (tid: track id) // used for adding new item to playlist

                // type param // album = '1', formdlfeat = '2', collection = '3', friend = '4', search_tracks = '5', search_artists = '7', pop (popular) = '8', history = '9', 'my' = 10 , artist = '11',  personalpl = '14', formaddpl = '17', myRadio = '19', pplRecTracks = '26'
                inline QUrl playAudioUrl(QString tid/*, int item_type = 5*/) { return audioUrl(QStringLiteral("play"), QUrlQuery(QStringLiteral("tid=%2").arg(tid))); } // params: (tid: track id) (pid: ? ('null')) (type: type as int val) (position: position in list (0))
                inline QUrl play30AudioUrl() { return audioUrl(QStringLiteral("play30")); } // this request sended after 30 sec playing ? // params: (tid: track id) (pid: ? ('null')) (type: type as name val) (position: position in list (0))

                inline QUrl tunersUrl() { return audioUrl(QStringLiteral("myTuners")); } // params: (locale: 'ru')  need to check pagination
                inline QUrl radioUrl(QString /*tuner*/) { return audioUrl(QStringLiteral("myRadio")); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs

                QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                    if (predicate.isEmpty() || limitations.by_popularity())
                        return lQuery(popAudioUrl(), QueryRules(QStringLiteral("tracks"), requestLimit()));
                    else {
                        if (limitations.by_artists())
                            return lQuery(searchArtistsUrl(predicate), QueryRules(QStringLiteral("artists"), requestLimit(), OD_SEARCH_LIMIT));
                        else if (limitations.by_songs())
                            return lQuery(searchTracksUrl(predicate), QueryRules(QStringLiteral("tracks"), requestLimit(), OD_SEARCH_LIMIT));
                        else
                            return lQuery(searchUrl(predicate), QueryRules(QStringLiteral("tracks"), requestLimit(), OD_SEARCH_LIMIT));
                    }
                }
            public:
                inline virtual ~RequestApi() {}

                inline QUrl initAudioUrl() { return audioUrl(QStringLiteral("init")); }
                inline QUrl myAudioUrl(const QString & uid) { return audioUrl(QStringLiteral("my"), QUrlQuery(QStringLiteral("uid=") % uid)); } // params: (uid: sets for friend request) and pagination attrs
                QJsonObject userInfo(const QString & uid) {
                    if (uid.isEmpty()) {
                        qDebug() << initAudioUrl();
                        return Manager::prepare() -> getJson(initAudioUrl());
                    } else
                        return Manager::prepare() -> getJson(myAudioUrl(uid));
                }

                inline QUrl playlistAudioUrl(const QString & pid) { return audioUrl(QStringLiteral("my"), QUrlQuery(QStringLiteral("pid=") % pid)); } // params: (pid: playlist id) and pagination attrs
                QJsonArray playlistInfo(QString & pid, int count) {
                    return lQuery(
                        playlistAudioUrl(pid),
                        QueryRules(QStringLiteral("tracks"), requestLimit(), qMin(count, OD_OFFSET_LIMIT))
                    );
                }
            };
        }
    }
}

#endif // OD_REQUEST_API
