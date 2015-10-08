#ifndef IPLAYER
#define IPLAYER

#include <qurl.h>

#include "itrackable.h"
#include "iequalizable.h"

#include "player_statuses.h"

class IPlayer : public IEqualizable, public ITrackable {
    Q_OBJECT

    PlayerState pstate;
    QTimer * itimer;
    uint volumeVal, panVal;
protected:
    void updateState(PlayerState new_state);

    virtual QString title() const { return media_url.toString(); }

    virtual bool playProcessing(uint startMili) = 0;
    inline void playPostprocessing() { updateState(PlayingState); }
    virtual bool resumeProcessing() = 0;
    virtual bool pauseProcessing() = 0;
    virtual bool stopProcessing() = 0;

    virtual uint recalcCurrentPosProcessing() = 0;
    virtual bool newPosProcessing(uint newPos) = 0;
    virtual bool newVolumeProcessing(uint newVol) = 0;
    virtual bool newPanProcessing(int newPan) = 0;

    inline void duration(uint newDuration) {
        ITrackable::setMaxProgress(newDuration);
        emit durationChanged((max_duration = newDuration));
    }
    virtual uint maxVolume() const = 0;
    inline virtual uint slidePercentage() const { return 10; }

    virtual inline bool seekingBlocked() { return false; }
    inline bool seekable() { return !seekingBlocked() && max_duration > 0 && (state() == PlayingState || state() == PausedState); }

    QUrl media_url;
    uint max_pos, max_duration;

public:
    explicit IPlayer(QWidget * parent);
    virtual ~IPlayer() {}

    inline void setMedia(const QUrl & url) {
        media_url = url;
        updateState(InitState);
        max_duration = 0;
    }

    inline bool isInitiating() { return pstate == InitState; }
    inline bool isPlayed() { return pstate == PlayingState; }
    inline bool isPaused() { return pstate == PausedState; }
    inline bool isStopped() { return pstate == StoppedState; }

    inline PlayerState state() const { return pstate; }

    virtual uint position() const = 0;
    uint duration() const { return max_duration; }
    inline uint volume() const { return volumeVal; }
    inline int pan() const { return panVal; }

    inline virtual void openTimeOut(float /*secLimit*/) { /*stub*/ }
    inline virtual void proxy(const QString & /*proxyStr*/ = QString()) { /*stub*/ }

    inline void prebufferingLevel(float level = 1) { emit prebufferingChanged(level); }
signals:
    void playbackEnded();

    void stateChanged(const PlayerState &);
    void statusChanged(const PlayerStatus &);

    void panChanged(int);
    void volumeChanged(uint);
    void positionChanged(uint);
    void durationChanged(uint);

    void prebufferingChanged(float level); // 0 .. 1

public slots:
    void play(uint startMili = 0, uint maxDuration = 0);
    void pause();
    void stop();

    void slidePosForward();
    void slidePosBackward();
    void slideVolForward();
    void slideVolBackward();

    void position(uint newPos);
    inline void volume(uint newVol) {
        newVolumeProcessing(volumeVal = newVol);
        emit volumeChanged(newVol);
    }
    inline void pan(int newPan) {
        newPanProcessing(panVal = newPan);
        emit panChanged(newPan);
    }

    void endOfPlayback() {
        position(0);
        pause();
        emit statusChanged(EndOfMedia);
    }
protected slots:
    void recalcPosition() { position(recalcCurrentPosProcessing()); }
};

#endif // IPLAYER
