#ifndef JSON_FIELDS_H
#define JSON_FIELDS_H

#include <qstringbuilder.h>

namespace Core {
    const QString JSON_TYPE_CONTAINER_LOADABLE = QLatin1String("0");
    const QString JSON_TYPE_MEDIA_TYPE = QLatin1String("1");
    const QString JSON_TYPE_MEDIA_ART = QLatin1String("2");
    const QString JSON_TYPE_CONTAINER_FETCHABLE = QLatin1String("3");
    const QString JSON_TYPE_ALBUMS = QLatin1String("4");
    const QString JSON_TYPE_TAGS = QLatin1String("5");
    const QString JSON_TYPE_LABELS = QLatin1String("6");
    const QString JSON_TYPE_ARTISTS = QLatin1String("7");

    const QString JSON_TYPE_INFO = QLatin1String("a");
    const QString JSON_TYPE_BYTES_SIZE = QLatin1String("b");
    const QString JSON_TYPE_CHILDS = QLatin1String("c");
    const QString JSON_TYPE_DURATION = QLatin1String("d");
    const QString JSON_TYPE_EXTENSION = QLatin1String("e");
    const QString JSON_TYPE_GENRE_ID = QLatin1String("g");
//    const QString JSON_TYPE_ = QLatin1String("f");
    const QString JSON_TYPE_ITEM_TYPE = QLatin1String("i");
    const QString JSON_TYPE_SONG_ID = QLatin1String("h");
    const QString JSON_TYPE_BPM = QLatin1String("m");
    const QString JSON_TYPE_PATH = QLatin1String("p");
    const QString JSON_TYPE_REFRESH_PATH = QLatin1String("r");
    const QString JSON_TYPE_STATE = QLatin1String("s");
    const QString JSON_TYPE_ERROR = QLatin1String("k");
    const QString JSON_TYPE_OWNER_ID = QLatin1String("o");
    const QString JSON_TYPE_TITLE = QLatin1String("t");
    const QString JSON_TYPE_ID = QLatin1String("u");
//    const QString JSON_TYPE_VIDEO_PATH = QLatin1String("v");
    const QString JSON_TYPE_CUE_MAP = QLatin1String("w");
    const QString JSON_TYPE_START_POS = QLatin1String("x");
    const QString JSON_TYPE_PARTIAL = QLatin1String("y");
    const QString JSON_TYPE_TITLE_CACHES = QLatin1String("z");
    const QString JSON_TYPE_CONTAINER_ITEMS_COUNT = QLatin1String("l");
    const QString JSON_TYPE_TAB_NAME = QLatin1String("n");
    const QString JSON_TYPE_TAB_SETTINGS = QLatin1String("set");
}

#endif // JSON_FIELDS_H
