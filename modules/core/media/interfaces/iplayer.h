#ifndef IPLAYER
#define IPLAYER

#include <qobject.h>
#include "itrackable.h"
#include "player_states.h"

class IPlayer : ITrackable {
public:
    inline explicit IPlayer(QObject * parent) : ITrackable(parent) {
        qRegisterMetaType<PlayerState>("PlayerState");
    }
    virtual ~IPlayer();

    inline void setMedia(const QUrl & url) {
        media_url = url;
        pstate = InitState;
    }

    void play();
    void pause();
    void stop();
    void setProgress(uint pos);
    void setMaxProgress(uint maxPos);

    inline bool isInitiating() { return pstate == InitState; }
    inline bool isPlayed() { return pstate == PlayingState; }
    inline bool isPaused() { return pstate == PausedState; }
    inline bool isStopped() { return pstate == StoppedState; }

    inline PlayerState state() const { return pstate; }

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
        pstate = new_state;
        ITrackable::updateState(isPlayed(), isPaused(), isStopped());
    }

    PlayerState pstate;
};

#endif // IPLAYER
