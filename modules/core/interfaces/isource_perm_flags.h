#ifndef ISOURCE_PERM_FLAGS
#define ISOURCE_PERM_FLAGS

namespace Core {
    enum Permissions : int {
        perm_none = 0,
        perm_site = 1,
        perm_api = 2
    };

    enum PermitRequest {
        pr_none                 = 0,

        pr_search_media         = 1,
        pr_search_objects       = 2, // user or group

        pr_media_content        = 4,
        pr_object_content       = 8, // user or group
//        pr_feed                 = 8,
        pr_pack                 = 16, // charts // tops // etc ...
        pr_user_recommendations = 32,
        pr_artist_recommendations
                                = 64,
        pr_item_recommendations = 128
    };
}

#endif // ISOURCE_PERM_FLAGS
