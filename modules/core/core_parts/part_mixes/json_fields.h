#ifndef JSON_FIELDS_H
#define JSON_FIELDS_H

#include <qstringbuilder.h>

namespace Core {
    const QString JSON_TYPE_INFO = QStringLiteral("a");
    const QString JSON_TYPE_BYTES_SIZE = QStringLiteral("b");
    const QString JSON_TYPE_CHILDS = QStringLiteral("c");
    const QString JSON_TYPE_DURATION = QStringLiteral("d");
    const QString JSON_TYPE_EXTENSION = QStringLiteral("e");
    const QString JSON_TYPE_ARTIST_IDS = QStringLiteral("f");
    const QString JSON_TYPE_GENRE_ID = QStringLiteral("g");
    const QString JSON_TYPE_ITEM_TYPE = QStringLiteral("i");
    const QString JSON_TYPE_SONG_ID = QStringLiteral("h");
    const QString JSON_TYPE_BPM = QStringLiteral("m");
    const QString JSON_TYPE_PATH = QStringLiteral("p");
    const QString JSON_TYPE_REFRESH_PATH = QStringLiteral("r");
    const QString JSON_TYPE_STATE = QStringLiteral("s");
    const QString JSON_TYPE_ERROR = QStringLiteral("k");
    const QString JSON_TYPE_OWNER_ID = QStringLiteral("o");
    const QString JSON_TYPE_TITLE = QStringLiteral("t");
    const QString JSON_TYPE_ID = QStringLiteral("u");
    const QString JSON_TYPE_VIDEO_PATH = QStringLiteral("v");
//    const QString JSON_TYPE_SUB_TYPE = QStringLiteral("w");
    const QString JSON_TYPE_START_POS = QStringLiteral("x");
    const QString JSON_TYPE_PARTIAL = QStringLiteral("y");
    const QString JSON_TYPE_TITLE_CACHES = QStringLiteral("z");
    const QString JSON_TYPE_CONTAINER_ITEMS_COUNT = QStringLiteral("l");
    const QString JSON_TYPE_TAB_NAME = QStringLiteral("n");
    const QString JSON_TYPE_TAB_SETTINGS = QStringLiteral("set");
}

#endif // JSON_FIELDS_H
