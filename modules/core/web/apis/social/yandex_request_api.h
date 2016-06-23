#ifndef YANDEX_REQUEST_API
#define YANDEX_REQUEST_API

#include "yandex_api_keys.h"
#include "modules/core/web/interfaces/iapi.h"

#define YANDEX_SEARCH_LIMIT 200

namespace Core {
    namespace Web {
        namespace Yandex {
            class RequestApi : public IApi {
            protected:
                RequestApi() {} // uid should be in format track_id:album_id (25358347:2988319)

                // url_api_v1 % account-status.jsx?experiments={"feedTracksByGenre":"default","loserFeedGenreTop":"chart-second","feedBrick":"default","rotorLoserHistoryToIchwill":"default","rotorLessDashboards":"default","rotorPersonalStationOn":"on","rotorLoserAllSessions":"default","rotorNewGenres":"default","userRadio":"ichwill","playlistOfTheDayAds":"ads","feedNewCandidates":"default","feedNewAlbumsOfLikedLabel":"default","rotorGreatestHits":"default","rotorOtherUserRadio":"default","feedDoublingBlocks":"doubling","rotorTrueDeboyanizator":"on","feedMissedTracksByArtist":"default","feedLogNotShownBlocks":"default","feedMixed":"default","useNewItemStorage":"default","rbt":"on","feedRecentTrackLikeTracks":"default","userFeed":"feed22","rotorIchwillRadio":"radio5_new__penalize_like_ratio__use_new_personal_station","webAds":"no-ads","showRadio":"default","feedRareArtist":"default","feedDither":"default","rotorStations":"simple_dashboard_formula_expected-time_track_finished_1_spav__1","rotorAds":"no-ads","feedWizardAsFilter":"default","feedDropTimeoutsAtMidnight":"on","radioTab":"place","artistWikiFacts":"on","feedArtistByFriends":"default","feedPlaylistOfTheDayFormula":"feed_potd_5_anti_boyan_100","personalTops":"enabled","feedAfishaNewApi":"on","paywall":"default","feedLongMemory":"default","rotorSafeTopTracksCount":"twelve","rotorPizza":"fallback","rotorPersonalStationOnMobile":"on","rotorPersonalProgressComposite":"default","feedTriggers":"default","feedCounters":"default","rotorCandidatesSampling":"default","feedUseNewWizard":"on","similarities":"default","genreRadio":"rotator","recommendedArtists":"ichwill_similar_artists","recommendedTracks":"recommended_tracks_by_artist_from_history","recommendedAlbumsOfFavoriteGenre":"recent","recommendedSimilarArtists":"default","recommendedArtistsWithArtistsFromHistory":"force_recent","adv":"d","loserArtistsWithArtists":"off"}&lang=ru&external-domain=music.yandex.ua&overembed=false // return keys: success, reason
                // url_api_v2 % auth?external-domain=music.yandex.ua&overembed=no&__t=1451819854469 // returns csrf, device_id, logged, premium, invalid
                bool sessionIsValid() { return false; }

                QString calcKey(const QString & path, const QString & s) {
                    QString str = path.mid(1) + s;

//                    str.replace(QStringLiteral("\r\n"), QStringLiteral("\n"))

                    QCryptographicHash hash(QCryptographicHash::Md5);
                    hash.addData((QStringLiteral("XGRlBW9FXlekgbPrRHuSiA") % str).toLatin1());
                    return QString(hash.result().toHex().toLower());
                }


//                PolyQueryRules rules(
//                    int offset = 0, int items_limit = SOUNDCLOUD_ITEMS_LIMIT, int pages_limit = SOUNDCLOUD_PAGES_LIMIT,
//                    int per_request = SOUNDCLOUD_PER_REQUEST_LIMIT,
//                    ApiCallIterType call_type = call_iter_type_page)
//                {
//                    return PolyQueryRules(
//                        call_type,
//                        call_iter_method_offset,
//                        qMin(items_limit, SOUNDCLOUD_ITEMS_LIMIT),
//                        qMin(pages_limit, SOUNDCLOUD_PAGES_LIMIT),
//                        tkn_limit,
//                        qMin(qMin(per_request, items_limit), SOUNDCLOUD_PER_REQUEST_LIMIT),
//                        tkn_offset,
//                        offset
//                    );
//                }


                //this maybe should presents in any request: external-domain=music.yandex.ua&overembed=false
                // lang=ru

                //experiments={"feedTracksByGenre":"default","loserFeedGenreTop":"chart-second","feedBrick":"default","rotorLoserHistoryToIchwill":"default","rotorLessDashboards":"default","rotorPersonalStationOn":"on","rotorLoserAllSessions":"default","rotorNewGenres":"default","userRadio":"ichwill","playlistOfTheDayAds":"ads","feedNewCandidates":"default","feedNewAlbumsOfLikedLabel":"default","rotorGreatestHits":"default","rotorOtherUserRadio":"default","feedDoublingBlocks":"doubling","rotorTrueDeboyanizator":"on","feedMissedTracksByArtist":"default","feedLogNotShownBlocks":"default","feedMixed":"default","useNewItemStorage":"default","rbt":"on","feedRecentTrackLikeTracks":"default","userFeed":"feed22","rotorIchwillRadio":"radio5_new__penalize_like_ratio__use_new_personal_station","webAds":"no-ads","showRadio":"default","feedRareArtist":"default","feedDither":"default","rotorStations":"simple_dashboard_formula_expected-time_track_finished_1_spav__1","rotorAds":"no-ads","feedWizardAsFilter":"default","feedDropTimeoutsAtMidnight":"on","radioTab":"place","artistWikiFacts":"on","feedArtistByFriends":"default","feedPlaylistOfTheDayFormula":"feed_potd_5_anti_boyan_100","personalTops":"enabled","feedAfishaNewApi":"on","paywall":"default","feedLongMemory":"default","rotorSafeTopTracksCount":"twelve","rotorPizza":"fallback","rotorPersonalStationOnMobile":"on","rotorPersonalProgressComposite":"default","feedTriggers":"default","feedCounters":"default","rotorCandidatesSampling":"default","feedUseNewWizard":"on","similarities":"default","genreRadio":"rotator","recommendedArtists":"ichwill_similar_artists","recommendedTracks":"recommended_tracks_by_artist_from_history","recommendedAlbumsOfFavoriteGenre":"recent","recommendedSimilarArtists":"default","recommendedArtistsWithArtistsFromHistory":"force_recent","adv":"d","loserArtistsWithArtists":"off"}

                // array of {id, name, cover, genres, ratings}
                inline QString artistInfoUrl(const QString & artistId) { return url_api_v1 + (QStringLiteral("artist.jsx?artist=%1").arg(artistId)); }
                //artist // similar // albums // alsoAlbums // tracks // radio // alsoAlbumIds // albumIds // concerts
                inline QString artistTracksUrl(const QString & artistId) { return url_api_v1 + QStringLiteral("artist.jsx?artist=%1&what=tracks&sort=&dir=").arg(artistId); }
                inline QString artistAlbumsUrl(const QString & artistId) { return url_api_v1 + QStringLiteral("artist.jsx?artist=%1&what=albums&sort=&dir=").arg(artistId); }
                inline QString artistSimilarUrl(const QString & artistId) { return url_api_v1 + QStringLiteral("artist.jsx?artist=%1&what=similar&sort=&dir=").arg(artistId); }

                // id, title, year, cover, coverUri, genre, artists, labels: [{id, name}], volumes: [[{id, realId, title, durationMs, fileSize, storageDir, available}, ..]]
                inline QString albumInfoUrl(const QString & albumId) { return url_api_v1 + QStringLiteral("albums.jsx?albumIds=%1").arg(albumId); }

                // __t=1451812081991
                // array of { id, realId, title, fileSize, storageDir, durationMs, embedPlayback, artists: {id, name, decomposed: ["feat or vs", {id, name}, ..]}}
                inline QString traksUrl(const QStringList & trackIds) {
                    QString res = url_api_v2 % QStringLiteral("tracks?");

                    for(QStringList::ConstIterator trackId = trackIds.constBegin(); trackId != trackIds.constEnd(); trackId++)
                        res = res % QStringLiteral("tracks[]=") % (*trackId) % '&';

                    return res;
                }

                //__t=1451823793515
                // codec, bitrate, src, gain
                inline QString trackDownloadingUrl(const QString & trackId) { return url_api_v2 + QStringLiteral("track/%1/download").arg(trackId); }


                //filter = albums: genre, pager: {total, page, perPage}, albums,
                //filter = : genre, tracks, artists, albums, pics
                inline QString genresUrl(QString genre = QString(), const QString & filter = QString()) { return url_api_v1 + QStringLiteral("genre.jsx?genre=%1&filter=%2").arg(genre, filter); }


                inline QString topUrl(const QString & filter = QString(), QString genre = QStringLiteral("all")) { return url_api_v1 + QStringLiteral("top.jsx?genre=%1&filter=%2").arg(genre, filter); }

                inline QString searchUrl(const QString & predicate, const QString & sources = QStringLiteral("all")) { return url_api_v1 + QStringLiteral("music-search.jsx?text=%1&type=%2").arg(predicate, sources); }

                // ncrnd=0.9069681936002026
                // days: [{day, tracksToPlay: [], events: [{type = 'recomended-genre-radio', title: {}, genres: [id, artists:[{}], albums: []]}, {type='genre-top, tracks: [], genre, title: {type, text}'}]}]
                inline QString feedsUrl() { return url_api_v1 + QStringLiteral("feed.jsx?nocheck="); }

                // title, kind, trackCount, tracks, owner: {uid, login, name, verified}, cover: {type, dir, version, uri}, trackIds, modified
                inline QString playlistUrl(const QString & owner) { return url_api_v1 + QStringLiteral("playlist.jsx?owner=%1&kinds=1267&light=true").arg(owner); }

                QJsonValue popular(const SearchLimit & limits) {
                    return sRequest(
                        topUrl(tkn_tracks, limits.genre),
                        call_type_json
                    ).value(tkn_tracks).toArray();

//                    return sQuery(topUrl(tkn_tracks, limits.genre)).value(tkn_tracks).toArray();
                }

                QJsonValue searchProc(const SearchLimit & limits) {
                    if (limits.predicate.isEmpty() || limits.by_popularity()) {
                        return popular(limits);
                    } else {
                        QString tkn = limits.by_artists() ? tkn_artists : tkn_tracks;

                        return sRequest(
                            searchUrl(limits.predicate, tkn),
                            call_type_json
                        ).value(tkn).toObject().value(QStringLiteral("items")).toArray();

//                        if (limits.by_artists())
//                            return sQuery(searchUrl(limits.predicate, tkn_artists)).value(tkn_artists).toObject().value(tkn).toArray();
//                        else
//                            return sQuery(searchUrl(limits.predicate, tkn_tracks)).value(tkn_tracks).toObject().value(tkn).toArray();
                    }
                }
            public:
                inline virtual ~RequestApi() {}

//                inline QUrl initAudioUrl() { return audioUrl(path_audio_init); }
//                inline QUrl myAudioUrl(const QString & uid) { return audioUrl(tkn_my, QUrlQuery(tkn_uid_eq % uid)); } // params: (uid: sets for friend request) and pagination attrs
//                QJsonObject userInfo(const QString & uid) {
//                    if (uid.isEmpty()) {
//                        qDebug() << initAudioUrl();
//                        return Manager::prepare() -> getJson(initAudioUrl());
//                    } else
//                        return Manager::prepare() -> getJson(myAudioUrl(uid));
//                }

//                inline QUrl playlistAudioUrl(const QString & pid) { return audioUrl(tkn_my, QUrlQuery(tkn_pid_eq % pid)); } // params: (pid: playlist id) and pagination attrs
//                QJsonArray playlistInfo(QString & pid, int count) {
//                    return lQuery(
//                        playlistAudioUrl(pid),
//                        QueryRules(tkn_tracks, requestLimit(), qMin(count, OD_OFFSET_LIMIT))
//                    );
//                }

//                QJsonArray audioInfo(const QStringList & uids) { // take only tracks - response also contains artists and albums arrays
//                    return sQuery(customAudioUrl(uids)).value(tkn_tracks).toArray();
//                }
            };
        }
    }
}

#endif // YANDEX_REQUEST_API
