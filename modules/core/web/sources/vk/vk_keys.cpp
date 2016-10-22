#include "vk_keys.h"

namespace Core {
    namespace Web {
        namespace Vk {
            extern const QString tkn_version           = QStringLiteral("v");
            extern const QString tkn_display           = QStringLiteral("display");
            extern const QString tkn_client_id         = QStringLiteral("client_id");
            extern const QString tkn_response_type     = QStringLiteral("response_type");
            extern const QString tkn_scope             = QStringLiteral("scope");
            extern const QString tkn_redirect_uri      = QStringLiteral("redirect_uri");
            extern const QString tkn_access_token      = QStringLiteral("access_token");
            extern const QString tkn_offset            = QStringLiteral("offset");
            extern const QString tkn_limit             = QStringLiteral("count");
            extern const QString tkn_response          = QStringLiteral("response");
            extern const QString tkn_finished          = QStringLiteral("finished");
            extern const QString tkn_user_id           = QStringLiteral("user_id");
            extern const QString tkn_expires_in        = QStringLiteral("expires_in");
            extern const QString tkn_error             = QStringLiteral("error");
            extern const QString tkn_error_description = QStringLiteral("error_description_key");
            extern const QString tkn_error_msg         = QStringLiteral("error_msg");
            extern const QString tkn_error_code        = QStringLiteral("error_code");
            extern const QString tkn_captcha_img       = QStringLiteral("captcha_img");
            extern const QString tkn_captcha_sid       = QStringLiteral("captcha_sid");
            extern const QString tkn_captcha           = QStringLiteral("captcha_key");
            extern const QString tkn_execute           = QStringLiteral("execute");
            extern const QString tkn_code              = QStringLiteral("code");
//            extern const QString tkn_albums            = QStringLiteral("albums");
//            extern const QString tkn_video_albums      = QStringLiteral("video_albums");
//            extern const QString tkn_audios            = QStringLiteral("audios");
//            extern const QString tkn_audio_list        = QStringLiteral("audio_list");
//            extern const QString tkn_video_list        = QStringLiteral("video_list");
//            extern const QString tkn_groups            = QStringLiteral("groups");
//            extern const QString tkn_friends           = QStringLiteral("friends");
            extern const QString tkn_albums_offset     = QStringLiteral("albums_offset");
            extern const QString tkn_albums_finished   = QStringLiteral("albums_finished");
            extern const QString tkn_url               = QStringLiteral("url");
            extern const QString tkn_id                = QStringLiteral("id");
            extern const QString tkn_owner_id          = QStringLiteral("owner_id");
            extern const QString tkn_artist            = QStringLiteral("artist");
            extern const QString tkn_title             = QStringLiteral("title");
            extern const QString tkn_duration          = QStringLiteral("duration");
            extern const QString tkn_genre_id          = QStringLiteral("genre_id");
            extern const QString tkn_fields            = QStringLiteral("fields");
            extern const QString tkn_counters          = QStringLiteral("counters");
            extern const QString tkn_user_ids          = QStringLiteral("user_ids");
            extern const QString tkn_screen_name       = QStringLiteral("screen_name");
            extern const QString tkn_object_id         = QStringLiteral("object_id");
            extern const QString tkn_type              = QStringLiteral("type");
            extern const QString tkn_lyrics_id         = QStringLiteral("lyrics_id");
            extern const QString tkn_email             = QStringLiteral("email");
            extern const QString tkn_password          = QStringLiteral("pass");
            extern const QString tkn_date              = QStringLiteral("date");
            extern const QString tkn_items             = QStringLiteral("items");
            extern const QString tkn_time_sep          = QStringLiteral(" : ");
            extern const QString tkn_q                 = QStringLiteral("q");
            extern const QString tkn_photo             = QStringLiteral("photo_200");
            extern const QString tkn_group_ids         = QStringLiteral("group_ids");
            extern const QString tkn_domain            = QStringLiteral("domain");
            extern const QString tkn_video_art         = QStringLiteral("photo_320");
            extern const QString tkn_player            = QStringLiteral("player");



            extern const QString url_redirect_uri      = QStringLiteral("https://oauth.vk.com/blank.html");
            extern const QString url_api_base          = QStringLiteral("https://api.vk.com/method/");
            extern const QString url_site_base         = QStringLiteral("http://vk.com/");
            extern const QString url_auth              = QStringLiteral("https://oauth.vk.com/authorize");


            extern const QString path_groups_by_id     = QStringLiteral("groups.getById");
            extern const QString path_groups_search    = QStringLiteral("groups.search");
            extern const QString path_users_search     = QStringLiteral("users.search");
            extern const QString path_user_info        = QStringLiteral("users.get");
            extern const QString path_resole_user      = QStringLiteral("utils.resolveScreenName");
            extern const QString path_lyrics           = QStringLiteral("audio.getLyrics");


            extern const QString val_str_true          = QStringLiteral("1");
            extern const QString val_str_false         = QStringLiteral("0");
            extern const QString val_name              = QStringLiteral("Vk");
            extern const QString val_api_call_limit    = QStringLiteral("20");
            extern const QString val_version           = QStringLiteral("5.52");
            extern const QString val_page_token        = QStringLiteral("page");
            extern const QString val_client_token      = QStringLiteral("4332211");
            extern const QString val_response_token    = QStringLiteral("token");
            extern const QString val_scope             = QStringLiteral("audio,video,friends,groups,offline"); // nohttps,docs,photos
            extern const QString val_user_fields       = tkn_photo % QStringLiteral(",") % tkn_screen_name;
            extern const QString val_group_types       = QStringLiteral("group,page");

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
