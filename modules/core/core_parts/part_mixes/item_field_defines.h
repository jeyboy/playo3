#ifndef ITEM_FIELD_DEFINES
#define ITEM_FIELD_DEFINES

#include "item_errors.h"
#include "json_fields.h"
#include "item_state.h"

namespace Core {
    #define IURL Qt::UserRole + 1
    #define IFULLPATH IURL + 1
    #define IPLAYABLE IFULLPATH + 1
    #define IATTRS IPLAYABLE + 1
    #define IEXECCOUNTS IATTRS + 1
    #define ITREEPATH IEXECCOUNTS + 1
    #define ITREESTR ITREEPATH + 1
    #define ISTATERESTORE ITREESTR + 1
    #define IUID ISTATERESTORE + 1
    #define ITYPE IUID + 1
    #define ISPOILITEM ITYPE + 1

    #define ITITLE Qt::DisplayRole
    #define IEXTENSION Qt::UserRole - 1
    #define IPATH IEXTENSION - 1
    #define IFOLDER IPATH - 1
    #define ISTATE IFOLDER - 1
    #define IINFO ISTATE - 1
    #define ITITLESCACHE IINFO - 1
    #define IADDFONT ITITLESCACHE - 1
    #define IREMOTE IADDFONT - 1
    #define IERROR IREMOTE - 1

    #define REMOTE_DND_URL QUrl::fromLocalFile("REMOTE:/")
    #define SHARE_DELIMITER QStringLiteral("^")
//    #define SHARE_TYPE_VK QStringLiteral("vk")
//    #define SHARE_TYPE_SOUNDCLOUD QStringLiteral("sc")

    #define NO_SOURCE_ITEM_ATTRS(name) \
        {\
            {JSON_TYPE_ITEM_TYPE, dt_web}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_ERROR, err_not_existed} \
        }

    #define LOCAL_ITEM_ATTRS(path, name) \
        {\
            {JSON_TYPE_ITEM_TYPE, dt_local}, \
            {JSON_TYPE_PATH, path}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
        }
}

#endif // ITEM_FIELD_DEFINES
