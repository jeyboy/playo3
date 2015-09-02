#ifndef AUDIO_PLAYER_STATES
#define AUDIO_PLAYER_STATES

#include <qobject.h>

namespace AudioPlayer {
    enum MediaStateFlags {
        InitState,
        StoppedState,
        PlayingState,
        PausedState,
        UnknowState
    };
    typedef QFlags<MediaStateFlags> MediaState;

    enum MediaStatusFlags {
        UnknownMediaStatus,
        NoMedia,
        NoRemoteMedia,
        LoadingMedia,
        LoadedMedia,
        StalledMedia,
        EndOfMedia,
        InvalidMedia
    };
    typedef QFlags<MediaStatusFlags> MediaStatus;

//    Q_ENUMS(MediaStateFlags)
//    Q_ENUMS(MediaStatusFlags)
}

#endif // AUDIO_PLAYER_STATES
