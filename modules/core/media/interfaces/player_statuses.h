#ifndef PLAYER_STATUSES
#define PLAYER_STATUSES

enum PlayerStatus {
    UnknownMediaStatus,
    NoMedia,
    NoRemoteMedia,
    StalledMedia,
    LoadingMedia,
    LoadedMedia,
    StartOfMedia,
    EndOfMedia,
    InvalidMedia
};

#endif // PLAYER_STATUSES
