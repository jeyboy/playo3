#include "yandex_keys.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            extern const QString url_root =                         QLatin1String("https://music.yandex.%1/");

            extern const QString url_site_v1 =                      url_root % QLatin1String("handlers/");
            extern const QString url_site_v2 =                      url_root % QLatin1String("api/v2.0/handlers/");
            extern const QString url_site_v2_1 =                    url_root % QLatin1String("api/v2.1/handlers/");

            extern const QString tkn_album =                        QLatin1String("album");
            extern const QString tkn_albums =                       QLatin1String("albums");
            extern const QString tkn_artists =                      QLatin1String("artists");
            extern const QString tkn_track =                        QLatin1String("track");
            extern const QString tkn_tracks =                       QLatin1String("tracks");
            extern const QString tkn_playlist =                     QLatin1String("playlist");
            extern const QString tkn_playlists =                    QLatin1String("playlists");
            extern const QString tkn_promotions =                   QLatin1String("promotions");
            extern const QString tkn_items =                        QLatin1String("items");
            extern const QString tkn_videos =                       QLatin1String("videos");

            extern const QString tkn_duration_ms =                  QLatin1String("durationMs");
            extern const QString tkn_id =                           QLatin1String("id");
            extern const QString tkn_fileSize =                     QLatin1String("fileSize");
            extern const QString tkn_name =                         QLatin1String("name");
            extern const QString tkn_title =                        QLatin1String("title");
            extern const QString tkn_full_title =                   QLatin1String("ftitle");
            extern const QString tkn_storageDir =                   QLatin1String("storageDir");
            extern const QString tkn_genre =                        QLatin1String("genre");
            extern const QString tkn_year =                         QLatin1String("year");
            extern const QString tkn_uid =                          QLatin1String("uid");
            extern const QString tkn_login =                        QLatin1String("login");
            extern const QString tkn_owner =                        QLatin1String("owner");
            extern const QString tkn_filter =                       QLatin1String("filter");
            extern const QString tkn_kind =                         QLatin1String("kind");

            extern const QString tkn_available =                    QLatin1String("available");
            extern const QString tkn_coverUri =                     QLatin1String("coverUri");
            extern const QString tkn_cover =                        QLatin1String("cover");

            extern const QString tkn_page =                         QLatin1String("page");
            extern const QString tkn_pager =                        QLatin1String("pager");
            extern const QString tkn_rep =                          QLatin1String("rep");

//            extern const QString val_resend_sms_title =             QLatin1String("Resend sms");
//            extern const QString val_sms_code_title =               QLatin1String("Code from sms");
            extern const QString val_name =                         QLatin1String("Yandex");
            extern const QString val_key_set =                      QLatin1String("XGRlBW9FXlekgbPrRHuSiA");
        }
    }
}
