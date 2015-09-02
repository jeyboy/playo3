#ifndef AUDIO_PLAYER_STATE
#define AUDIO_PLAYER_STATE

#include <qobject.h>

#include "bass.h"

void
    #ifdef Q_OS_WIN
        __stdcall
    #endif
        endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
void
    #ifdef Q_OS_WIN
        __stdcall
    #endif
        endTrackDownloading(HSYNC, DWORD, DWORD, void * user);

class AudioPlayerState : public QObject {
    Q_OBJECT
    //    Q_ENUMS(MediaStateFlags)
    //    Q_ENUMS(MediaStatusFlags)
public:
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

    virtual ~AudioPlayerState() {}

    inline MediaState state() const { return currentState; }

    inline bool isPlayed() const { return currentState == PlayingState; }
    inline bool isPaused() const { return currentState == PausedState; }
    inline bool isStoped() const { return currentState == StoppedState; }

    inline void finishRemoteFileDownloading() { prevDownloadPos = 1; }
signals:
    void stateChanged(MediaState);
    void mediaStatusChanged(MediaStatus);
    void playbackEnded();
    void downloadEnded();

protected:
    AudioPlayerState(QObject * parent = 0) : QObject(parent), currentState(StoppedState) {
        qRegisterMetaType<AudioPlayerState::MediaStatus>("MediaStatus");
        qRegisterMetaType<AudioPlayerState::MediaState>("MediaState");
    }

    void proceedErrorState() {
        currentState = UnknowState;
        switch(BASS_ErrorGetCode()) {
            case BASS_ERROR_FILEFORM: {
                emit mediaStatusChanged(InvalidMedia);
            break;}
            case BASS_ERROR_FILEOPEN: {
                emit mediaStatusChanged(NoMedia);
            break;}
            default: emit mediaStatusChanged(StalledMedia);
        }
    }

    MediaState currentState;
    HSYNC syncHandle, syncDownloadHandle;
};

#endif // AUDIO_PLAYER_STATE
