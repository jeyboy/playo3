#ifndef IPLAYER
#define IPLAYER

#include <qtimer.h>
#include <qurl.h>

#include "itrackable.h"
#include "player_states.h"

class IPlayer : public QTimer, public ITrackable {
public:
    inline IPlayer(QWidget * parent) : ITrackable(parent), max_duration(0) {
        qRegisterMetaType<PlayerState>("PlayerState");
        setInterval(1000);
    }
    virtual ~IPlayer();

    inline void setMedia(const QUrl & url) {
        media_url = url;
        pstate = InitState;
        max_duration = 0;
    }

    inline bool isInitiating() { return pstate == InitState; }
    inline bool isPlayed() { return pstate == PlayingState; }
    inline bool isPaused() { return pstate == PausedState; }
    inline bool isStopped() { return pstate == StoppedState; }

    inline PlayerState state() const { return pstate; }

    virtual uint position() const = 0;
    uint duration() const { return max_duration; }
    virtual uint volume() const = 0;
    virtual int pan() const = 0;

signals:
    void panChanged(int);
    void volumeChanged(uint);
    void positionChanged(uint);
    void durationChanged(uint);

public slots:
    void play(uint startMili = 0);
    void pause();
    void stop();

    void slidePosForward();
    void slidePosBackward();
    void slideVolForward();
    void slideVolBackward();

    inline void position(uint newPos) {
        newPosProcessing(newPos);
        ITrackable::setProgress(newPos);
        emit positionChanged(newPos);
    }
    inline void volume(uint newVol) {
        newVolumeProcessing(newVol);
        emit volumeChanged(newVol);
    }
    inline void pan(int newPan) {
        newPanProcessing(newPan);
        emit panChanged(newPan);
    }

protected:
    virtual QString title() const { return media_url.toString(); }

    virtual void playProcessing(uint startMili) = 0;
    virtual void resumeProcessing() = 0;
    virtual void pauseProcessing() = 0;
    virtual void stopProcessing() = 0;

    virtual void newPosProcessing(uint newPos) = 0;
    virtual void newVolumeProcessing(uint newVol) = 0;
    virtual void newPanProcessing(int newPan) = 0;

    inline void duration(uint newDuration) {
        ITrackable::setMaxProgress(newDuration);
        emit durationChanged((max_duration = newDuration));
    }
    virtual uint maxVolume() const = 0;
    inline virtual uint slidePercentage() const { return 10; }

    virtual inline bool seekingBlocked() { return false; }
    inline bool seekable() { seekingBlocked() || max_duration > 0 && state() == PlayingState || state() == PausedState; }

    QUrl media_url;
    uint max_pos, max_duration;
private:
    void updateState(PlayerState new_state) {
        pstate = new_state;
        ITrackable::updateState(isPlayed(), isPaused(), isStopped());
    }

    PlayerState pstate;
};

#endif // IPLAYER
