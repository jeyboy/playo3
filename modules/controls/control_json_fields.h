#ifndef CONTROL_JSON_FIELDS_H
#define CONTROL_JSON_FIELDS_H

namespace Controls {
    enum JsonType {
        tab_item_count = "l",
        tab_name = "n",
        tab_settings = "set",
        vk_tab_settings = "vk"
    };
}

/// a - item info
/// b - item bytes size
/// c - children
/// d - duration
/// e - extension
/// g - genreID
/// i - item type
////// l - tab items count
////// n - tab name
/// m - beat per minute
/// p - path
/// s - state
////// set - tab settings
/// o - web item owner id
/// t - title
/// u - web item id
////// vk - vk settings
///


#endif // CONTROL_JSON_FIELDS_H
