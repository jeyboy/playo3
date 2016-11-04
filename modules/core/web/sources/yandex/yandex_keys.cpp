#include "yandex_keys.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            extern const QString url_root =                         QStringLiteral("https://music.yandex.%1/");

            extern const QString url_site_v1 =                      url_root % QStringLiteral("handlers/");
            extern const QString url_site_v2 =                      url_root % QStringLiteral("api/v2.0/handlers/");
            extern const QString url_site_v2_1 =                    url_root % QStringLiteral("api/v2.1/handlers/");

            extern const QString tkn_album =                        QStringLiteral("album");
            extern const QString tkn_albums =                       QStringLiteral("albums");
            extern const QString tkn_artists =                      QStringLiteral("artists");
            extern const QString tkn_track =                        QStringLiteral("track");
            extern const QString tkn_tracks =                       QStringLiteral("tracks");
            extern const QString tkn_playlist =                     QStringLiteral("playlist");
            extern const QString tkn_playlists =                    QStringLiteral("playlists");
            extern const QString tkn_promotions =                   QStringLiteral("promotions");
            extern const QString tkn_items =                        QStringLiteral("items");
            extern const QString tkn_videos =                       QStringLiteral("videos");

            extern const QString tkn_duration_ms =                  QStringLiteral("durationMs");
            extern const QString tkn_id =                           QStringLiteral("id");
            extern const QString tkn_fileSize =                     QStringLiteral("fileSize");
            extern const QString tkn_name =                         QStringLiteral("name");
            extern const QString tkn_title =                        QStringLiteral("title");
            extern const QString tkn_full_title =                   QStringLiteral("ftitle");
            extern const QString tkn_storageDir =                   QStringLiteral("storageDir");
            extern const QString tkn_genre =                        QStringLiteral("genre");
            extern const QString tkn_year =                         QStringLiteral("year");
            extern const QString tkn_uid =                          QStringLiteral("uid");
            extern const QString tkn_login =                        QStringLiteral("login");
            extern const QString tkn_owner =                        QStringLiteral("owner");
            extern const QString tkn_filter =                       QStringLiteral("filter");
            extern const QString tkn_kind =                         QStringLiteral("kind");

            extern const QString tkn_available =                    QStringLiteral("available");
            extern const QString tkn_coverUri =                     QStringLiteral("coverUri");
            extern const QString tkn_cover =                        QStringLiteral("cover");

            extern const QString tkn_page =                         QStringLiteral("page");
            extern const QString tkn_pager =                        QStringLiteral("pager");
            extern const QString tkn_rep =                          QStringLiteral("rep");

//            extern const QString val_resend_sms_title =             QStringLiteral("Resend sms");
//            extern const QString val_sms_code_title =               QStringLiteral("Code from sms");
            extern const QString val_name =                         QStringLiteral("Yandex");
            extern const QString val_key_set =                      QStringLiteral("XGRlBW9FXlekgbPrRHuSiA");
            extern const QString val_default_locale =               QStringLiteral("ua");
        }
    }
}
