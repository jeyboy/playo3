#ifndef AUDIO_PLAYER_STATE
#define AUDIO_PLAYER_STATE

#include <qobject.h>

#include "bass.h"
#include "audio_player_states.h"

namespace AudioPlayer {
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


    class State : virtual public QObject {
    public:
        virtual ~State() {}

        inline bool isPlayed() const { return currentState == PlayingState; }
        inline bool isPaused() const { return currentState == PausedState; }
        inline bool isStoped() const { return currentState == StoppedState; }
    signals:
        void stateChanged(MediaState);
        void mediaStatusChanged(MediaStatus);
        void playbackEnded();
        void downloadEnded();

    protected:
        State() : currentState(StoppedState) {
            qRegisterMetaType<MediaStatus>("MediaStatus");
            qRegisterMetaType<MediaState>("MediaState");
        }

        virtual unsigned long chId() const = 0;

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
}

#endif // AUDIO_PLAYER_STATE
