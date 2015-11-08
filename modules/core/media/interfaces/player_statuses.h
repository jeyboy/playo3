#ifndef PLAYER_STATUSES
#define PLAYER_STATUSES

enum PlayerStatus {
    InvalidMedia = -3,
    UnknownMediaStatus,
    StalledMedia,
    NoMedia = 0,
    InitMedia,
    LoadingMedia,
    LoadedMedia,
    PlaingMedia,
    EndPlaingMedia,
    CloseMedia,
};

#endif // PLAYER_STATUSES
