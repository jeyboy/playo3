#ifndef MODEL_ITEM_JSON_FIELDS_H
#define MODEL_ITEM_JSON_FIELDS_H

namespace ModelItems {
    enum JsonType {
        info = "a",
        bytes_size = "b",
        childs = "c",
        duration = "d",
        extension = "e",
        genre_id = "g",
        item_type = "i",
        bpm = "m",
        path = "p",
        state = "s",
        owner_id = "o",
        title = "t",
        uid = "u"
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


#endif // MODEL_ITEM_JSON_FIELDS_H
