#ifndef IPLAYER
#define IPLAYER

#include <qurl.h>

#include "itrackable.h"
#include "iequalizable.h"

#include "player_statuses.h"
#include "modules/core/media/interfaces/imediainfo.h"

class IPlayer : public IEqualizable, public ITrackable {
    Q_OBJECT

    PlayerState pstate;
    QTimer * itimer;
    int volumeVal, panVal;
    float prebuffering_level;
    bool muted, looped;
protected:
    void updateState(PlayerState new_state);

    virtual QString title() const { return media_url.toString(); }

    virtual bool playProcessing(int startMili, bool paused = false) = 0;
    void playPostprocessing();
    virtual bool resumeProcessing() = 0;
    virtual bool pauseProcessing() = 0;
    virtual bool stopProcessing() = 0;

    virtual int recalcCurrentPosProcessing() = 0;
    virtual bool newPosProcessing(int newPos) = 0;
    virtual bool newVolumeProcessing(int newVol) = 0;
    virtual bool newPanProcessing(int newPan) = 0;

    inline void duration(int newDuration) {
        ITrackable::setMaxProgress(newDuration);
        emit durationChanged((max_duration = newDuration));
    }
    virtual int maxVolume() const = 0;
    inline virtual int slidePercentage() const { return 10; }

    virtual inline bool seekingBlocked() { return false; }
    inline bool seekable() { return !seekingBlocked() && max_duration > 0 && (state() == PlayingState || state() == PausedState); }

    QUrl media_url;
    int max_pos, max_duration;

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

    virtual int position() const = 0;
    int duration() const { return max_duration; }
    inline int volume() const { return muted ? 0 : volumeVal; }
    inline int pan() const { return panVal; }

    inline virtual void openTimeOut(float /*secLimit*/) { /*stub*/ }
    inline virtual void proxy(const QString & /*proxyStr*/ = QString()) { /*stub*/ }

    inline void prebufferingLevel(float level = 1) { emit prebufferingChanged(prebuffering_level = level); }
    inline float prebufferingLevel() const { return prebuffering_level; }

    virtual bool fileInfo(const QUrl & /*uri*/, IMediaInfo * /*info*/) { return false; }
signals:
    void playbackEnded();

    void stateChanged(const PlayerState &);
    void statusChanged(const PlayerStatus &);

    void panChanged(int);
    void volumeChanged(int);
    void positionChanged(int);
    void durationChanged(int);

    void prebufferingChanged(float level); // 0 .. 1

public slots:
    void play(int startMili = 0, bool paused = false, int maxDuration = 0);
    void pause();
    void playPause() { isPlayed() ? pause() : play(); }
    void stop();

    void slidePosForward();
    void slidePosBackward();
    void slideVolForward();
    void slideVolBackward();

    void setPosition(int newPos);
    void mute(bool enable = false);
    void loop(bool enable = false) { looped = enable; }
    inline void setVolume(int newVol) {
        newVolumeProcessing(volumeVal = newVol);
        emit volumeChanged(newVol);
    }
    inline void setPan(int newPan) {
        newPanProcessing(panVal = newPan);
        emit panChanged(newPan);
    }

    void endOfPlayback() {
        emit statusChanged(EndOfMedia);

        if (!looped)
            pause();

        setPosition(0);
    }
protected slots:
    void recalcPosition() { setPosition(recalcCurrentPosProcessing()); }
};

#endif // IPLAYER
