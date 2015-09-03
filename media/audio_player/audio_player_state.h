#ifndef AUDIO_PLAYER_STATE
#define AUDIO_PLAYER_STATE

#include <qobject.h>

#include "bass.h"

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

    class State : public QObject {
        Q_OBJECT
    public:
        inline State(QObject * parent = 0) : QObject(parent) {}
        virtual ~State() {}

        inline bool isPlayed() const { return currentState == PlayingState; }
        inline bool isPaused() const { return currentState == PausedState; }
        inline bool isStoped() const { return currentState == StoppedState; }

        inline void finishRemoteFileDownloading() { prevDownloadPos = 1; }

        //from 0 to 1
        float getRemoteFileDownloadPosition();
        inline float getSize() const { return size; }

        inline MediaState state() const { return currentState; }

    signals:
        void stateChanged(MediaState);
        void mediaStatusChanged(MediaStatus);

    protected:
        State() : currentState(StoppedState) {
            qRegisterMetaType<MediaStatus>("MediaStatus");
            qRegisterMetaType<MediaState>("MediaState");
        }

        void proceedErrorState();

        virtual unsigned long chId() const = 0;

        MediaState currentState;
        HSYNC syncHandle, syncDownloadHandle;
        float prevDownloadPos;
        float size;
    };
}

#endif // AUDIO_PLAYER_STATE
