#ifndef ITEM_FIELD_DEFINES
#define ITEM_FIELD_DEFINES

#include <qstringbuilder.h>

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
    #define SHARE_DELIMITER QLatin1String("^")

    #define CONTAINER_ATTRS(ctype, name) \
        {\
            {JSON_TYPE_ITEM_TYPE, ctype}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_STATE, DEFAULT_CONTAINER_STATE} \
        }

    #define LOCAL_CONTAINER_ATTRS(ctype, path, name) \
        {\
            {JSON_TYPE_ITEM_TYPE, ctype}, \
            {JSON_TYPE_PATH, path}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_STATE, DEFAULT_CONTAINER_STATE} \
        }

    #define REMOTE_CONTAINER_ATTRS(ctype, name, id) \
        {\
            {JSON_TYPE_ITEM_TYPE, ctype}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_STATE, DEFAULT_CONTAINER_STATE} \
        }

    #define LOADABLE_CONTAINER_ATTRS(id, name, loadable_attrs) \
        {\
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, dt_playlist}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_CONTAINER_LOADABLE, loadable_attrs}, \
            {JSON_TYPE_STATE, DEFAULT_CONTAINER_STATE} \
        }


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
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE} \
        }

    #define LOCAL_CUE_ITEM_ATTRS(path, name, extension, start_pos, is_parted) \
        {\
            {JSON_TYPE_ITEM_TYPE, dt_local_cue}, \
            {JSON_TYPE_PATH, path}, \
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_EXTENSION, extension}, \
            {JSON_TYPE_START_POS, start_pos}, \
            {JSON_TYPE_PARTIAL, is_parted} \
        }

    #define WEB_ITEM_ATTRS(id, uri, name, subtype, refresh, extension, media_type) \
        {\
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, subtype}, \
            {JSON_TYPE_PATH, uri}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_EXTENSION, extension}, \
            {JSON_TYPE_REFRESH_PATH, refresh}, \
            {JSON_TYPE_MEDIA_TYPE, media_type} \
        }

    #define VK_ITEM_ATTRS(id, uri, name, owner, refresh, duration, media_type) \
        {\
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, dt_web_vk}, \
            {JSON_TYPE_PATH, uri}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_OWNER_ID, owner}, \
            {JSON_TYPE_REFRESH_PATH, refresh}, \
            {JSON_TYPE_DURATION, duration}, \
            {JSON_TYPE_MEDIA_TYPE, media_type} \
        }

    #define YANDEX_ITEM_ATTRS(id, name, refresh, duration, media_type) \
        {\
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, dt_web_yandex}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_REFRESH_PATH, refresh}, \
            {JSON_TYPE_DURATION, duration}, \
            {JSON_TYPE_MEDIA_TYPE, media_type} \
        }

    #define YOUTUBE_ITEM_ATTRS(id, name, refresh) \
        {\
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, dt_web_youtube}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_REFRESH_PATH, refresh}, \
            {JSON_TYPE_MEDIA_TYPE, dmt_video} \
        }

    #define SC_ITEM_ATTRS(id, refresh_path, uri, name, owner, duration, extension, media_type) \
        {\
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, dt_web_sc}, \
            {JSON_TYPE_PATH, uri}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_OWNER_ID, owner}, \
            {JSON_TYPE_DURATION, duration}, \
            {JSON_TYPE_EXTENSION, extension}, \
            {JSON_TYPE_MEDIA_TYPE, media_type}, \
            {JSON_TYPE_REFRESH_PATH, refresh_path} \
        }

    #define OD_ITEM_ATTRS(id, name, refresh, duration, size, media_type) \
        {\
            {JSON_TYPE_TITLE, name}, \
            {JSON_TYPE_ID, id}, \
            {JSON_TYPE_ITEM_TYPE, dt_web_od}, \
            {JSON_TYPE_STATE, DEFAULT_ITEM_STATE}, \
            {JSON_TYPE_DURATION, duration}, \
            {JSON_TYPE_BYTES_SIZE, size}, \
            {JSON_TYPE_REFRESH_PATH, refresh}, \
            {JSON_TYPE_MEDIA_TYPE, media_type} \
        }
}

#endif // ITEM_FIELD_DEFINES
