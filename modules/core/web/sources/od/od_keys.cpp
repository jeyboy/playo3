#include "od_keys.h"

namespace Core {
    namespace Web {
        namespace Od {
            extern const QString url_root =                         QStringLiteral("http://ok.ru/");
            extern const QString url_base_auth =                    QStringLiteral("https://www.ok.ru/");
            extern const QString url_base_audio =                   QStringLiteral("http://wmf1.odnoklassniki.ru/");

            extern const QString path_sid =                         QStringLiteral("web-api/music/conf");
            extern const QString path_auth =                        QStringLiteral("https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=%1&st.password=%2&st.remember=on&st.iscode=false");
            extern const QString path_auth2 =                       QStringLiteral("dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=");

            extern const QString path_audio_init =                  QStringLiteral("init");
            extern const QString path_audio_play =                  QStringLiteral("play");
            extern const QString path_audio_play_30_sec =           QStringLiteral("play30");
            extern const QString path_audio_info =                  QStringLiteral("custom");
            extern const QString path_audio_history =               QStringLiteral("history");
            extern const QString path_audio_popular =               QStringLiteral("pop");
            extern const QString path_audio_popular_tracks =        QStringLiteral("popTracks");
            extern const QString path_audio_add_to_playlist_form =  QStringLiteral("formaddpl");
            extern const QString path_audio_radio =                 QStringLiteral("myRadio");
            extern const QString path_audio_tuners_for_radio =      QStringLiteral("myTuners");
            extern const QString path_audio_is_downloaded =         QStringLiteral("isDownloaded");
            extern const QString path_audio_downloaded =            QStringLiteral("downloaded");
            extern const QString path_audio_collections =           QStringLiteral("collections");
            extern const QString path_audio_by_artist_id =          QStringLiteral("artist");
            extern const QString path_audio_by_album_id =           QStringLiteral("album");
            extern const QString path_audio_search_albums =         QStringLiteral("albums");
            extern const QString path_audio_search_artists =        QStringLiteral("artists");
            extern const QString path_audio_search_tracks =         QStringLiteral("tracks");
            extern const QString path_audio_search =                QStringLiteral("relevant");
            extern const QString path_audio_playlist_add =          QStringLiteral("playlistsAdd");
            extern const QString path_audio_playlist_del =          QStringLiteral("playlistsRemove");
            extern const QString path_audio_playlists =             QStringLiteral("playlistsGet");
            extern const QString path_audio_to_playlist =           QStringLiteral("like");
            extern const QString path_audio_del_from_playlist =     QStringLiteral("dislike");


            extern const QString tkn_ids =                          QStringLiteral("ids=");
            extern const QString tkn_uid_eq =                       QStringLiteral("uid=");
            extern const QString tkn_pid_eq =                       QStringLiteral("pid=");
            extern const QString tkn_tid_eq =                       QStringLiteral("tid=");
            extern const QString tkn_q_eq =                         QStringLiteral("q=");

            extern const QString tkn_my =                           QStringLiteral("my");
            extern const QString tkn_artists =                      QStringLiteral("artists");
            extern const QString tkn_tracks =                       QStringLiteral("tracks");
            extern const QString tkn_coma_dot =                     QStringLiteral(";");
            extern const QString tkn_authcode =                     QStringLiteral("AUTHCODE");
            extern const QString tkn_httpsdata =                    QStringLiteral("httpsdata");
            extern const QString tkn_form_error =                   QStringLiteral("st.error");
            extern const QString tkn_client_hash =                  QStringLiteral("clientHash");
            extern const QString tkn_md5 =                          QStringLiteral("md5");
            extern const QString tkn_play =                         QStringLiteral("play");
            extern const QString tkn_captcha =                      QStringLiteral("st.mobileCaptcha");
            extern const QString tkn_secret =                       QStringLiteral("secret");
            extern const QString tkn_code =                         QStringLiteral("code");
            extern const QString tkn_sid =                          QStringLiteral("sid");
            extern const QString tkn_header_user_agent =            QStringLiteral("User-Agent");
            extern const QString tkn_error =                        QStringLiteral("error");
            extern const QString tkn_offset =                       QStringLiteral("start");
            extern const QString tkn_limit =                        QStringLiteral("count"); // separate key for request limit
            extern const QString tkn_count =                        QStringLiteral("count"); // separate key for json vakue
            extern const QString tkn_chunk =                        QStringLiteral("chunk");
            extern const QString tkn_jsessionid =                   QStringLiteral("jsessionid=");

            extern const QString tkn_dash =                         QStringLiteral(" - ");
            extern const QString tkn_id =                           QStringLiteral("id");
            extern const QString tkn_name =                         QStringLiteral("name");
            extern const QString tkn_duration =                     QStringLiteral("duration");
            extern const QString tkn_size =                         QStringLiteral("size");
            extern const QString tkn_ensemble =                     QStringLiteral("ensemble");

            extern const QString val_login_title =                  QStringLiteral("Odnoklassniki auth");
            extern const QString val_resend_sms_title =             QStringLiteral("Resend sms");
            extern const QString val_sms_code_title =               QStringLiteral("Code from sms");
            extern const QString val_name =                         QStringLiteral("Od");
        }
    }
}
