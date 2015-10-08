#ifndef PLAYER_STATUSES
#define PLAYER_STATUSES

enum PlayerStatus {
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

#endif // PLAYER_STATUSES
