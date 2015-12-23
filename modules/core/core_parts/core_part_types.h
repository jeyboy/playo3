#ifndef CORE_PART_TYPES_H
#define CORE_PART_TYPES_H

#include <qstringbuilder.h>

namespace Core {
    #define SIMPLE_FILE 1000
    #define PLAYLIST 1001
    #define CUE_FILE 1002
    #define WEB_FILE 2000
    #define VK_FILE 2100
    #define VK_PLAYLIST 2101
    #define SOUNDCLOUD_FILE 2200
    #define SOUNDCLOUD_PLAYLIST 2201
    #define OD_FILE 2300
    #define OD_PLAYLIST 2301

    #define SHARE_DELIMITER QStringLiteral("^")
}

#endif // CORE_PART_TYPES_H
