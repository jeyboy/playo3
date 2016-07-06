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

        pr_search               = 1,

        pr_media_content        = 2,
        pr_user_content         = 4,
        pr_feed                 = 8,
    //        pr_chart                = 16,
        pr_recommendations      = 32
    };
}

#endif // ISOURCE_PERM_FLAGS
