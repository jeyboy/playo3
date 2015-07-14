#ifndef JSON_FIELDS_H
#define JSON_FIELDS_H

#include <qstringbuilder.h>

namespace Playo3 {
    #define JSON_TYPE_INFO QStringLiteral("a")
    #define JSON_TYPE_BYTES_SIZE QStringLiteral("b")
    #define JSON_TYPE_CHILDS QStringLiteral("c")
    #define JSON_TYPE_DURATION QStringLiteral("d")
    #define JSON_TYPE_EXTENSION QStringLiteral("e")
    #define JSON_TYPE_ARTIST_UID QStringLiteral("f")
    #define JSON_TYPE_GENRE_ID QStringLiteral("g")
    #define JSON_TYPE_ITEM_TYPE QStringLiteral("i")
    #define JSON_TYPE_SONG_UID QStringLiteral("h")

    #define JSON_TYPE_BPM QStringLiteral("m")
    #define JSON_TYPE_PATH QStringLiteral("p")
    #define JSON_TYPE_STATE QStringLiteral("s")
    #define JSON_TYPE_OWNER_ID QStringLiteral("o")
    #define JSON_TYPE_TITLE QStringLiteral("t")
    #define JSON_TYPE_UID QStringLiteral("u")

    #define JSON_TYPE_VIDEO_PATH QStringLiteral("v")

    #define JSON_TYPE_TITLE_CACHES QStringLiteral("z")

    #define JSON_TYPE_CONTAINER_ITEMS_COUNT QStringLiteral("l")

    #define JSON_TYPE_TAB_NAME QStringLiteral("n")
    #define JSON_TYPE_TAB_SETTINGS QStringLiteral("set")
}

#endif // JSON_FIELDS_H
