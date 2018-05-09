#include "vk_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            extern const QString tkn_version           = QLatin1String("v");
            extern const QString tkn_display           = QLatin1String("display");
            extern const QString tkn_client_id         = QLatin1String("client_id");
            extern const QString tkn_response_type     = QLatin1String("response_type");
            extern const QString tkn_scope             = QLatin1String("scope");
            extern const QString tkn_redirect_uri      = QLatin1String("redirect_uri");
            extern const QString tkn_access_token      = QLatin1String("access_token");
            extern const QString tkn_offset            = QLatin1String("offset");
            extern const QString tkn_limit             = QLatin1String("count");
            extern const QString tkn_response          = QLatin1String("response");
            extern const QString tkn_finished          = QLatin1String("finished");
            extern const QString tkn_user_id           = QLatin1String("user_id");
            extern const QString tkn_expires_in        = QLatin1String("expires_in");
            extern const QString tkn_error             = QLatin1String("error");
            extern const QString tkn_error_description = QLatin1String("error_description_key");
            extern const QString tkn_error_msg         = QLatin1String("error_msg");
            extern const QString tkn_error_code        = QLatin1String("error_code");
            extern const QString tkn_captcha_img       = QLatin1String("captcha_img");
            extern const QString tkn_captcha_sid       = QLatin1String("captcha_sid");
            extern const QString tkn_captcha           = QLatin1String("captcha_key");
            extern const QString tkn_execute           = QLatin1String("execute");
            extern const QString tkn_code              = QLatin1String("code");
//            extern const QString tkn_albums            = QLatin1String("albums");
//            extern const QString tkn_video_albums      = QLatin1String("video_albums");
//            extern const QString tkn_audios            = QLatin1String("audios");
//            extern const QString tkn_audio_list        = QLatin1String("audio_list");
//            extern const QString tkn_video_list        = QLatin1String("video_list");
//            extern const QString tkn_groups            = QLatin1String("groups");
//            extern const QString tkn_friends           = QLatin1String("friends");
            extern const QString tkn_albums_offset     = QLatin1String("albums_offset");
            extern const QString tkn_albums_finished   = QLatin1String("albums_finished");
            extern const QString tkn_url               = QLatin1String("url");
            extern const QString tkn_id                = QLatin1String("id");
            extern const QString tkn_owner_id          = QLatin1String("owner_id");
            extern const QString tkn_artist            = QLatin1String("artist");
            extern const QString tkn_title             = QLatin1String("title");
            extern const QString tkn_duration          = QLatin1String("duration");
            extern const QString tkn_genre_id          = QLatin1String("genre_id");
            extern const QString tkn_fields            = QLatin1String("fields");
            extern const QString tkn_counters          = QLatin1String("counters");
            extern const QString tkn_user_ids          = QLatin1String("user_ids");
            extern const QString tkn_screen_name       = QLatin1String("screen_name");
            extern const QString tkn_object_id         = QLatin1String("object_id");
            extern const QString tkn_type              = QLatin1String("type");
            extern const QString tkn_lyrics_id         = QLatin1String("lyrics_id");
            extern const QString tkn_email             = QLatin1String("email");
            extern const QString tkn_password          = QLatin1String("pass");
            extern const QString tkn_date              = QLatin1String("date");
            extern const QString tkn_items             = QLatin1String("items");
            extern const QString tkn_time_sep          = QLatin1String(" : ");
            extern const QString tkn_q                 = QLatin1String("q");
            extern const QString tkn_photo             = QLatin1String("photo_200");
            extern const QString tkn_group_ids         = QLatin1String("group_ids");
            extern const QString tkn_domain            = QLatin1String("domain");
            extern const QString tkn_video_art         = QLatin1String("photo_320");
            extern const QString tkn_player            = QLatin1String("player");



            extern const QString url_redirect_uri      = QLatin1String("https://oauth.vk.com/blank.html");
            extern const QString url_api_base          = QLatin1String("https://api.vk.com/method/");
            extern const QString url_site_base         = QLatin1String("http://vk.com/");
            extern const QString url_auth              = QLatin1String("https://oauth.vk.com/authorize");


            extern const QString path_groups_by_id     = QLatin1String("groups.getById");
            extern const QString path_groups_search    = QLatin1String("groups.search");
            extern const QString path_users_search     = QLatin1String("users.search");
            extern const QString path_user_info        = QLatin1String("users.get");
            extern const QString path_resole_user      = QLatin1String("utils.resolveScreenName");
            extern const QString path_lyrics           = QLatin1String("audio.getLyrics");


            extern const QString val_str_true          = QLatin1String("1");
            extern const QString val_str_false         = QLatin1String("0");
            extern const QString val_name              = QLatin1String("Vk");
            extern const QString val_api_call_limit    = QLatin1String("20");
            extern const QString val_version           = QLatin1String("5.52");
            extern const QString val_page_token        = QLatin1String("page");
            extern const QString val_client_token      = QLatin1String("4332211");
            extern const QString val_response_token    = QLatin1String("token");
            extern const QString val_scope             = QLatin1String("audio,video,friends,groups,offline"); // nohttps,docs,photos
            extern const QString val_user_fields       = tkn_photo % QLatin1String(",") % tkn_screen_name;
            extern const QString val_group_types       = QLatin1String("group,page");

            extern const QString query_track_playlists(const QString & owner_id) {
                return QString(
                    "var curr; var count = 5;"
                    "var folders_result = API.audio.getAlbums({"
                    "                count: count, "
                    "                offset: " % VK_DEFAULT_OFFSET_TEMPLATE % ", "
                    "                owner_id: " % owner_id % " "
                    "    });"
                    "var folders_result = folders_result.items;"
                    "var proceed_folders = {};"
                    "while(folders_result.length > 0) {"
                    "    curr = folders_result.pop();"
                    "    proceed_folders.push({"
                    "        " % tkn_id % ": curr.id,"
                    "        " % tkn_title % ": curr.title,"
                    "        " % tkn_items % ": API.audio.get({"
                    "            owner_id: " % owner_id % ","
                    "            album_id: curr.id"
                    "        }).items "
                    "    });"
                    "};"
                    "return { "
                    "    \"" + block_sets_audio + "\": proceed_folders, "
                    "    " % tkn_finished % ": (proceed_folders.length < count), "
                    "    " % tkn_offset % ": " % VK_DEFAULT_OFFSET_TEMPLATE % " %2b count"
                    "};"
                );
            }

            extern const QString query_video_playlists(const QString & owner_id) {
                return QString(
                    "var curr; var count = 5;"
                    "var folders_result = API.video.getAlbums({"
                    "                count: count, "
                    "                offset: " % VK_DEFAULT_OFFSET_TEMPLATE % ", "
                    "                owner_id: " % owner_id % ", "
                    "                extended: 1"
                    "    });"
                    "var folders_result = folders_result.items;"
                    "var proceed_folders = {};"
                    "while(folders_result.length > 0) {"
                    "    curr = folders_result.pop();"
                    "    proceed_folders.push({"
                    "        " % tkn_id % ": curr.id,"
                    "        " % tkn_title % ": curr.title,"
                    "        " % tkn_video_art % ": curr." % tkn_video_art % ","
                    "        " % tkn_items % ": API.video.get({"
                    "            owner_id: " % owner_id % ","
                    "            album_id: curr.id"
                    "        }).items "
                    "    });"
                    "};"
                    "return { "
                    "    \"" % block_sets_video % "\": proceed_folders, "
                    "    " % tkn_finished % ": (proceed_folders.length < count), "
                    "    " % tkn_offset % ": " % VK_DEFAULT_OFFSET_TEMPLATE % " %2b count"
                    "};"
                );
            }


            extern const QString query_user_groups_friends(const QString & owner_id) {
                return QString(
                        "var curr; var proceed_groups = [];"
                        "var groups = API.groups.get({"
                        "            owner_id: " % owner_id % ", "
                        "            count: 1000, "
                        "            extended: 1"
                        "    }).items;"
                        "while(groups.length > 0) {"
                        "    curr = groups.pop();"
                        "    proceed_groups.push({"
                        "        " % tkn_id % ": curr.id, "
                        "        " % tkn_title % ": curr.name,"
                        "        " % tkn_screen_name % ": curr.screen_name,"
                        "        " % tkn_photo % ": curr." % tkn_photo % ""
                        "    });"
                        "};"

                        "var friends = API.friends.get({"
                        "            user_id: " % owner_id % ", "
                        "            order: \"name\", "
                        "            fields: \"" % tkn_domain % ", " % val_user_fields % "\""
                        "    });"
                        "var proceed_friends = [];"
                        "if (friends.count > 0) { "
                        "    while(friends.items.length > 0) { "
                        "        curr = friends.items.pop();"
                        "        if (!curr.deactivated) {"
                        "           proceed_friends.push({ "
                        "               " % tkn_id % ": curr.id, "
                        "               " % tkn_title % ": curr.first_name %2b \" \" %2b curr.last_name, "
                        "               " % tkn_screen_name % ": curr." % tkn_domain % ", "
                        "               " % tkn_photo % ": curr." % tkn_photo % ""
                        "           }); "
                        "        }"
                        "    }; "
                        "};"

                        "return {"
                        "    \"" + block_friends + "\": proceed_friends, "
                        "    \"" + block_groups + "\": proceed_groups, "
                        "};"
                    );
            }


            extern const QString query_user_videos(const QString & owner_id) {
                return QString(
                    "var albums = []; var videos = []; var counter = 0;"
                    "var videos_resp; var rule = true;"

                    "while(rule){"
                    "   videos_resp = API.video.get ({"
                    "       owner_id: " % owner_id % ","
                    "       count: 200,"
                    "       offset: videos.length, "
                    "       extended: 1"
                    "   });"
                    "   videos = videos %2b videos_resp.items;"
                    "   rule = (counter = counter %2b 1) < 22 && (videos_resp.count != 0);"
                    "}"

                    "var albums_resp;"
                    "rule = true;"

                    "while(rule){"
                    "   albums_resp = API.video.getAlbums ({"
                    "       owner_id: " % owner_id % ","
                    "       count: 100,"
                    "       offset: albums.length, "
                    "       extended: 1"
                    "   });"

//                    "   var proceed_albums = {};"
//                    "   while(albums_resp.length > 0) {"
//                    "       curr = folders_result.pop();"
//                        "    proceed_folders.push({"
//                        "        " % tkn_id % ": curr.id,"
//                        "        " % tkn_title % ": curr.title,"
//                        "        " % tkn_items % ": API.audio.get({"
//                        "            owner_id: " % owner_id % ","
//                        "            album_id: curr.id"
//                        "        }).items "
//                        "    });"
//                        "};"

                    "   albums = albums %2b albums_resp.items;"
                    "   rule = (counter = counter %2b 1) < 25 && (albums_resp.count != 0);"
                    "}"

                    "return {"
                    "    \"" % block_items_video % "\": videos,"
                    "    \"" % block_sets_video % "\": albums"
                    "};"
                );
            }

            extern const QString query_user_tracks_groups_friends(const QString & owner_id) {
                return QString(
                    "var curr; var proceed_groups = [];"
                    "var groups = API.groups.get({"
                    "            owner_id: " % owner_id % ", "
                    "            count: 1000, "
                    "            extended: 1"
                    "    }).items;"
                    "while(groups.length > 0) {"
                    "    curr = groups.pop();"
                    "    proceed_groups.push({"
                    "        " % tkn_id % ": curr.id, "
                    "        " % tkn_title % ": curr.name,"
                    "        " % tkn_screen_name % ": curr.screen_name,"
                    "        " % tkn_photo % ": curr." % tkn_photo % ""
                    "    });"
                    "};"

                    "var friends = API.friends.get({"
                    "            user_id: " % owner_id % ", "
                    "            order: \"name\", "
                    "            fields: \"" % tkn_domain % ", " % val_user_fields % "\""
                    "    });"
                    "var proceed_friends = [];"
                    "if (friends.count > 0) { "
                    "    while(friends.items.length > 0) { "
                    "        curr = friends.items.pop();"                           
                    "        if (!curr.deactivated) {"
                    "           proceed_friends.push({ "
                    "               " % tkn_id % ": curr.id, "
                    "               " % tkn_title % ": curr.first_name %2b \" \" %2b curr.last_name, "
                    "               " % tkn_screen_name % ": curr." % tkn_domain % ", "
                    "               " % tkn_photo % ": curr." % tkn_photo % ""
                    "           }); "
                    "        }"
                    "    }; "
                    "};"

                    "return {"
                    "    \"" % block_items_audio % "\": API.audio.get({ "
                    "        count: 6000, owner_id: " % owner_id % ""
                    "    }).items,"
                    "    \"" % block_friends % "\": proceed_friends, "
                    "    \"" % block_groups % "\": proceed_groups, "
                    "};"
                );
            }


            extern const QString query_user_tracks_playlists_groups_friends(const QString & owner_id) {
                return QString(
                    "var curr; var proceed_groups = [];"
                    "var groups = API.groups.get({"
                    "            owner_id: " % owner_id % ", "
                    "            count: 1000, "
                    "            extended: 1"
                    "    }).items;"
                    "while(groups.length > 0) {"
                    "    curr = groups.pop();"
                    "    proceed_groups.push({"
                    "        " % tkn_id % ": curr.id, "
                    "        " % tkn_title % ": curr.name,"
                    "        " % tkn_screen_name % ": curr.screen_name,"
                    "        " % tkn_photo % ": curr." % tkn_photo % ""
                    "    });"
                    "};"

                    "var friends = API.friends.get({"
                    "            user_id: " % owner_id % ", "
                    "            order: \"name\", "
                    "            fields: \"" % tkn_domain % ", " % val_user_fields % "\""
                    "    });"
                    "var proceed_friends = [];"
                    "if (friends.count > 0) { "
                    "    while(friends.items.length > 0) { "
                    "        curr = friends.items.pop();"                           
                    "        if (!curr.deactivated) {"
                    "           proceed_friends.push({ "
                    "               " % tkn_id % ": curr.id, "
                    "               " % tkn_title % ": curr.first_name %2b \" \" %2b curr.last_name, "
                    "               " % tkn_screen_name % ": curr." % tkn_domain % ", "
                    "               " % tkn_photo % ": curr." % tkn_photo % ""
                    "           }); "
                    "        }"
                    "    }; "
                    "};"

                    "var folders_result = API.audio.getAlbums({"
                    "            count: " % val_api_call_limit % ", "
                    "            owner_id: " % owner_id % ""
                    "    });"
                    "var folders_count = folders_result.count;"
                    "var proceed_folders = {};"
                    "if (folders_count > 0) { "
                    "    while(folders_result.items.length > 0) { "
                    "        curr = folders_result.items.pop();"
                    "        proceed_folders.push({"
                    "            " % tkn_id % ": curr.id, "
                    "            " % tkn_title % ": curr.title, "
                    "            " % tkn_items % ": API.audio.get({ "
                    "                album_id: curr.id "
                    "            }).items "
                    "        });"
                    "    };"
                    "};"
                    "return {"
                    "    " % block_items_audio % ": API.audio.get({ "
                    "        count: 6000, owner_id: " % owner_id % ""
                    "    }).items,"
                    "    \"" % block_sets_audio % "\": proceed_folders, "
                    "    \"" % block_groups % "\": proceed_groups, "
                    "    \"" % block_friends % "\": proceed_friends, "
                    "    " % tkn_albums_offset % ": " % val_api_call_limit % ", "
                    "    " % tkn_albums_finished % ": (folders_count < " % val_api_call_limit % "), "
                    "};"
                );
            }

            extern const QString query_user_tracks_playlists(const QString & owner_id) {
                return QString(
                    "var folders_result = API.audio.getAlbums({ "
                    "            count: " % val_api_call_limit % ", "
                    "            owner_id: " % owner_id % ""
                    "        });"
                    "var folders_count = folders_result.count;"
                    "var sort_by_folders = {};"
                    "if (folders_count > 0) { "
                    "    while(folders_result.items.length > 0) { "
                    "        var curr = folders_result.items.pop(); "
                    "        sort_by_folders.push({"
                    "            " % tkn_id % ": curr.id, "
                    "            " % tkn_title % ": curr.title, "
                    "            " % tkn_items % ": API.audio.get({ "
                    "                owner_id: " % owner_id % ", "
                    "                album_id: curr.id"
                    "            }).items"
                    "        });"
                    "    };"
                    "};"
                    "return {"
                    "    \"" % block_items_audio % "\": API.audio.get({ "
                    "        count: 6000, "
                    "        owner_id: " % owner_id % ""
                    "    }).items, "
                    "    \"" % block_sets_audio % "\": sort_by_folders, "
                    "    " % tkn_albums_offset % ": " % val_api_call_limit % ", "
                    "    " % tkn_albums_finished % ": (folders_count < " % val_api_call_limit % ")"
                    "};"
                );
            }
        }
    }
}
