#ifndef PLAYLIST_TYPES
#define PLAYLIST_TYPES

namespace Core {
    enum PlaylistType {
        playlist_level = 1,
        playlist_level_tree = 2,
        playlist_tree = 3,
        playlist_vk = 4,
        playlist_soundcloud = 5,
        playlist_search = 6,
        playlist_vk_rel = 7,
        playlist_echo = 8,
        playlist_od = 9
    };

    enum PlaylistRelType {
        playlist_none_rel,
        playlist_user_rel,
        playlist_song_rel
    };
}

#endif // PLAYLIST_TYPES
