#include "od_keys.h"

namespace Core {
    namespace Web {
        namespace Od {
            extern const QString url_root =                         QLatin1String("http://ok.ru/");
            extern const QString url_base_auth =                    QLatin1String("https://www.ok.ru/");
            extern const QString url_base_audio =                   QLatin1String("http://wmf1.odnoklassniki.ru/");

            extern const QString path_sid =                         QLatin1String("web-api/music/conf");
            extern const QString path_auth =                        QLatin1String("https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://www.ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&amp;tkn=2039&st.fJS=on&st.screenSize=1920x1080&st.browserSize=621&st.flashVer=18.0.0&st.email=%1&st.password=%2&st.remember=on&st.iscode=false");
            extern const QString path_auth2 =                       QLatin1String("dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=");

            extern const QString path_audio_init =                  QLatin1String("init");
            extern const QString path_audio_play =                  QLatin1String("play");
            extern const QString path_audio_play_30_sec =           QLatin1String("play30");
            extern const QString path_audio_info =                  QLatin1String("custom");
            extern const QString path_audio_history =               QLatin1String("history");
            extern const QString path_audio_popular =               QLatin1String("pop");
            extern const QString path_audio_popular_tracks =        QLatin1String("popTracks");
            extern const QString path_audio_add_to_playlist_form =  QLatin1String("formaddpl");
            extern const QString path_audio_radio =                 QLatin1String("myRadio");
            extern const QString path_audio_tuners =                QLatin1String("myTuners");
            extern const QString path_audio_is_downloaded =         QLatin1String("isDownloaded");
            extern const QString path_audio_downloaded =            QLatin1String("downloaded");
            extern const QString path_audio_collection =            QLatin1String("collection");
            extern const QString path_audio_collections =           QLatin1String("collections");
            extern const QString path_audio_by_artist_id =          QLatin1String("artist");
            extern const QString path_audio_by_album_id =           QLatin1String("album");
            extern const QString path_audio_search_albums =         QLatin1String("albums");
            extern const QString path_audio_search_artists =        QLatin1String("artists");
            extern const QString path_audio_search_tracks =         QLatin1String("tracks");
            extern const QString path_audio_search =                QLatin1String("relevant");
            extern const QString path_audio_playlist_add =          QLatin1String("playlistsAdd");
            extern const QString path_audio_playlist_del =          QLatin1String("playlistsRemove");
            extern const QString path_audio_playlists =             QLatin1String("playlistsGet");
            extern const QString path_audio_to_playlist =           QLatin1String("like");
            extern const QString path_audio_del_from_playlist =     QLatin1String("dislike");
            extern const QString path_audio_similar_by_playlist =   QLatin1String("similarTracksForPlaylist");


            extern const QString tkn_ids =                          QLatin1String("ids");
            extern const QString tkn_uid =                          QLatin1String("uid");
            extern const QString tkn_pid =                          QLatin1String("pid");
            extern const QString tkn_tid =                          QLatin1String("tid");
            extern const QString tkn_q =                            QLatin1String("q");

            extern const QString tkn_me =                           QLatin1String("me");
            extern const QString tkn_my =                           QLatin1String("my");
            extern const QString tkn_tuners =                       QLatin1String("tuners");
            extern const QString tkn_friends =                      QLatin1String("friends");
            extern const QString tkn_artists =                      QLatin1String("artists");
            extern const QString tkn_tracks =                       QLatin1String("tracks");
            extern const QString tkn_albums =                       QLatin1String("albums");
            extern const QString tkn_playlists =                    QLatin1String("playlists");
            extern const QString tkn_collections =                  QLatin1String("collections");
            extern const QString tkn_coma_dot =                     QLatin1String(";");
            extern const QString tkn_authcode =                     QLatin1String("AUTHCODE");
            extern const QString tkn_httpsdata =                    QLatin1String("httpsdata");
            extern const QString tkn_form_error =                   QLatin1String("st.error");
            extern const QString tkn_client_hash =                  QLatin1String("clientHash");
            extern const QString tkn_md5 =                          QLatin1String("md5");
            extern const QString tkn_play =                         QLatin1String("play");
            extern const QString tkn_captcha =                      QLatin1String("st.mobileCaptcha");
            extern const QString tkn_secret =                       QLatin1String("secret");
            extern const QString tkn_code =                         QLatin1String("code");
            extern const QString tkn_sid =                          QLatin1String("sid");
            extern const QString tkn_error =                        QLatin1String("error");
            extern const QString tkn_offset =                       QLatin1String("start");
            extern const QString tkn_limit =                        QLatin1String("count"); // separate key for request limit
            extern const QString tkn_count =                        QLatin1String("count"); // separate key for json value
            extern const QString tkn_chunk =                        QLatin1String("chunk");
            extern const QString tkn_jsessionid =                   QLatin1String("jsessionid=");
            extern const QString tkn_header =                       QLatin1String("TKN");

            extern const QString tkn_full_name =                    QLatin1String("fullName");
            extern const QString tkn_id =                           QLatin1String("id");
            extern const QString tkn_owner =                        QLatin1String("owner");
            extern const QString tkn_name =                         QLatin1String("name");
            extern const QString tkn_duration =                     QLatin1String("duration");
            extern const QString tkn_size =                         QLatin1String("size");
            extern const QString tkn_ensemble =                     QLatin1String("ensemble");
            extern const QString tkn_art_url =                      QLatin1String("url4");
            extern const QString tkn_perma =                        QLatin1String("perma");
            extern const QString tkn_tuner =                        QLatin1String("tuner");
            extern const QString tkn_locale =                       QLatin1String("locale");

            extern const QString val_resend_sms_title =             QLatin1String("Resend sms");
            extern const QString val_sms_code_title =               QLatin1String("Code from sms");
            extern const QString val_name =                         QLatin1String("Od");
        }
    }
}
