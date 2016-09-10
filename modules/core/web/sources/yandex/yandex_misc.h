#ifndef YANDEX_MISC_H
#define YANDEX_MISC_H

#include <qstring.h>
#include <qcryptographichash.h>

#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/iuser_interaction.h"
#include "yandex_keys.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Misc : public virtual ISource, public virtual IQueriable, public virtual IUserInteraction {
                protected:
                    inline Misc() {}

                    // url_api_v1 % account-status.jsx?experiments={"feedTracksByGenre":"default","loserFeedGenreTop":"chart-second","feedBrick":"default","rotorLoserHistoryToIchwill":"default","rotorLessDashboards":"default","rotorPersonalStationOn":"on","rotorLoserAllSessions":"default","rotorNewGenres":"default","userRadio":"ichwill","playlistOfTheDayAds":"ads","feedNewCandidates":"default","feedNewAlbumsOfLikedLabel":"default","rotorGreatestHits":"default","rotorOtherUserRadio":"default","feedDoublingBlocks":"doubling","rotorTrueDeboyanizator":"on","feedMissedTracksByArtist":"default","feedLogNotShownBlocks":"default","feedMixed":"default","useNewItemStorage":"default","rbt":"on","feedRecentTrackLikeTracks":"default","userFeed":"feed22","rotorIchwillRadio":"radio5_new__penalize_like_ratio__use_new_personal_station","webAds":"no-ads","showRadio":"default","feedRareArtist":"default","feedDither":"default","rotorStations":"simple_dashboard_formula_expected-time_track_finished_1_spav__1","rotorAds":"no-ads","feedWizardAsFilter":"default","feedDropTimeoutsAtMidnight":"on","radioTab":"place","artistWikiFacts":"on","feedArtistByFriends":"default","feedPlaylistOfTheDayFormula":"feed_potd_5_anti_boyan_100","personalTops":"enabled","feedAfishaNewApi":"on","paywall":"default","feedLongMemory":"default","rotorSafeTopTracksCount":"twelve","rotorPizza":"fallback","rotorPersonalStationOnMobile":"on","rotorPersonalProgressComposite":"default","feedTriggers":"default","feedCounters":"default","rotorCandidatesSampling":"default","feedUseNewWizard":"on","similarities":"default","genreRadio":"rotator","recommendedArtists":"ichwill_similar_artists","recommendedTracks":"recommended_tracks_by_artist_from_history","recommendedAlbumsOfFavoriteGenre":"recent","recommendedSimilarArtists":"default","recommendedArtistsWithArtistsFromHistory":"force_recent","adv":"d","loserArtistsWithArtists":"off"}&lang=ru&external-domain=music.yandex.ua&overembed=false // return keys: success, reason
                    // url_api_v2 % auth?external-domain=music.yandex.ua&overembed=no&__t=1451819854469 // returns csrf, device_id, logged, premium, invalid
                    bool sessionIsValid() { return false; }

                    QString calcKey(const QString & path, const QString & s) {
                        QString str = path.mid(1) + s;
    //                    str.replace(QStringLiteral("\r\n"), QStringLiteral("\n"))

                        QCryptographicHash hash(QCryptographicHash::Md5);
                        hash.addData((val_key_set % str).toLatin1());
                        return QString(hash.result().toHex().toLower());
                    }
            };
        }
    }
}

#endif // YANDEX_MISC_H
