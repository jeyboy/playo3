#ifndef AUDIO_PLAYER_STATE
#define AUDIO_PLAYER_STATE

#include <qobject.h>

#ifdef Q_OS_WIN
    void __stdcall endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
    void __stdcall endTrackDownloading(HSYNC, DWORD, DWORD, void * user);
#else
    void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
    void endTrackDownloading(HSYNC, DWORD, DWORD, void * user);
#endif

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
signals:
    void stateChanged(MediaState);
    void mediaStatusChanged(MediaStatus);
    void playbackEnded();
    void downloadEnded();

protected slots:
    void started();
    inline void stoped() { currentState = StoppedState; }

protected:
    AudioPlayerState(QObject * parent = 0) : QObject(parent) {
        qRegisterMetaType<AudioPlayerState::MediaStatus>("MediaStatus");
        qRegisterMetaType<AudioPlayerState::MediaState>("MediaState");
    }

    MediaState currentState;
    HSYNC syncHandle, syncDownloadHandle;
};

#endif // AUDIO_PLAYER_STATE
