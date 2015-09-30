#ifndef IPLAYER
#define IPLAYER

#include <qobject.h>
#include "itrackable.h"

enum PlayerStateFlags {
    InitState,
    StoppedState,
    PlayingState,
    PausedState,
    UnknowState
};
typedef QFlags<PlayerStateFlags> PlayerState;

class IPlayer : ITrackable {
public:
    inline explicit IPlayer(QObject * parent) : ITrackable(parent) {}
    virtual ~IPlayer();

    inline void setMedia(const QUrl & url) {
        media_url = url;
        state = InitState;
    }

    void play();
    void pause();
    void stop();
    void setProgress(uint pos);
    void setMaxProgress(uint maxPos);

    inline bool isInitiating() { return state == InitState; }
    inline bool isPlayed() { return state == PlayingState; }
    inline bool isPaused() { return state == PausedState; }
    inline bool isStopped() { return state == StoppedState; }

protected:
    virtual void playProcessing() = 0;
    virtual void pauseProcessing() = 0;
    virtual void stopProcessing() = 0;

    virtual void setProgressProcessing(uint pos) = 0;
    virtual void setMaxProgressProcessing(uint max) = 0;

    QUrl media_url;
    uint max_pos;
private:
    void updateState(PlayerState new_state) {
        state = new_state;
        ITrackable::updateState(isPlayed(), isPaused(), isStopped());
    }

    PlayerState state;
};

#endif // IPLAYER
