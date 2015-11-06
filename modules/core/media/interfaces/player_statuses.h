#ifndef PLAYER_STATUSES
#define PLAYER_STATUSES

enum PlayerStatus {
    InvalidMedia = -4,
    UnknownMediaStatus = -3,
    StalledMedia = -2,
    NoRemoteMedia = -1,
    NoMedia = 0,
    InitMedia,
    LoadingMedia,
    LoadedMedia,
    PlaingMedia,
    EndPlaingMedia,
    CloseMedia,
};

#endif // PLAYER_STATUSES
