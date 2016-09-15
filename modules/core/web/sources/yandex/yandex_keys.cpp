#include "yandex_keys.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            extern const QString url_root =                         QStringLiteral("https://music.yandex.ua/");

            extern const QString url_site_v1 =                      url_root % QStringLiteral("handlers/");
            extern const QString url_site_v2 =                      url_root % QStringLiteral("api/v2.0/handlers/");


//            extern const QString url_base_auth =                    QStringLiteral("https://www.ok.ru/");
//            extern const QString url_base_audio =                   QStringLiteral("http://wmf1.odnoklassniki.ru/");

//            extern const QString path_sid =                         QStringLiteral("web-api/music/conf");
//            extern const QString path_auth =                        QStringLiteral("https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=%1&st.password=%2&st.remember=on&st.iscode=false");
//            extern const QString path_auth2 =                       QStringLiteral("dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=");

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

            extern const QString tkn_durationMs =                   QStringLiteral("durationMs");
            extern const QString tkn_id =                           QStringLiteral("id");
            extern const QString tkn_fileSize =                     QStringLiteral("fileSize");
            extern const QString tkn_name =                         QStringLiteral("name");
            extern const QString tkn_title =                        QStringLiteral("title");
            extern const QString tkn_full_title =                   QStringLiteral("ftitle");
            extern const QString tkn_storageDir =                   QStringLiteral("storageDir");
            extern const QString tkn_genre =                        QStringLiteral("genre");
            extern const QString tkn_year =                         QStringLiteral("year");

            extern const QString tkn_available =                    QStringLiteral("available");
            extern const QString tkn_coverUri =                     QStringLiteral("coverUri");
            extern const QString tkn_cover =                        QStringLiteral("cover");

            extern const QString tkn_page =                         QStringLiteral("page");
            extern const QString tkn_pager =                        QStringLiteral("pager");
            extern const QString tkn_rep =                          QStringLiteral("rep");

//            extern const QString val_login_title =                  QStringLiteral("Odnoklassniki auth");
//            extern const QString val_resend_sms_title =             QStringLiteral("Resend sms");
//            extern const QString val_sms_code_title =               QStringLiteral("Code from sms");
            extern const QString val_name =                         QStringLiteral("Yandex");

            extern const QString val_key_set =                      QStringLiteral("XGRlBW9FXlekgbPrRHuSiA");
        }
    }
}
