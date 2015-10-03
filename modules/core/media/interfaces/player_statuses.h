#ifndef PLAYER_STATUSES
#define PLAYER_STATUSES

#include <qflags.h>

enum MediaStatusFlags {
    UnknownMediaStatus,
    NoMedia,
    NoRemoteMedia,
    LoadingMedia,
    LoadedMedia,
    StartPlayingMedia,
    StalledMedia,
    EndOfMedia,
    InvalidMedia
};
typedef QFlags<MediaStatusFlags> PlayerStatus;

#endif // PLAYER_STATUSES
