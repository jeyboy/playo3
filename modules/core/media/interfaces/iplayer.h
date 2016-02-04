#ifndef IPLAYER
#define IPLAYER

#include <qurl.h>
#include <qdebug.h>

#include "itrackable.h"
#include "iequalizable.h"

#include "player_statuses.h"
#include "modules/core/media/interfaces/imediainfo.h"

#define VOLUME_MULTIPLIER 10000.0
#define POSITION_MULTIPLIER 1000.0
#define PAN_MULTIPLIER 1000.0

class IPlayer : public IEqualizable, public ITrackable {
    Q_OBJECT

    PlayerState pstate;
    QTimer * itimer;
    qint64 playPos, startPos, size;
    int volumeVal, panVal;
    float prebuffering_level;
    bool muted, looped;
protected:
    void updateState(PlayerState new_state);
    void updatePosition(qint64 newPos);

    virtual QString title() const { return media_url.toString(); }

    virtual bool playProcessing(bool paused = false) = 0;
    void playPostprocessing();
    virtual bool resumeProcessing() = 0;
    virtual bool pauseProcessing() = 0;
    virtual bool stopProcessing() = 0;

    virtual qint64 recalcCurrentPosProcessing() = 0;
    virtual bool newPosProcessing(qint64 newPos) = 0;
    virtual bool newVolumeProcessing(int newVol) = 0;
    virtual bool newPanProcessing(int newPan) = 0;
    virtual float prebufferingLevelCalc() = 0;
    virtual qint64 calcFileSize() = 0;

    void initFileSize() {
        size = calcFileSize();
        qDebug() << "FILESIZE" << size;
    }

    inline void setDuration(qint64 newDuration) {
        ITrackable::setMaxProgress(newDuration);
        emit durationChanged((max_duration = newDuration));
    }
    inline virtual int slidePercentage() const { return 10.0; }

    virtual inline bool seekingBlocked() { return false; }
    inline bool seekable() { return !seekingBlocked() && max_duration > 0 && (state() == PlayingState || state() == PausedState); }

    QUrl media_url;
    qint64 max_pos, max_duration;

public:
    explicit IPlayer(QWidget * parent);
    virtual ~IPlayer() {}

    inline QUrl media() { return media_url; }

    inline void setMedia(const QUrl & url, qint64 startMili = 0, qint64 maxDuration = 0, qint64 playStartMili = 0) {
        media_url = url;
        startPos = startMili;
        playPos = playStartMili;
        setDuration(maxDuration);
        updatePosition(startMili);
        updateState(url.isEmpty() ? UnknowState : InitState);
    }

    inline void updateMedia(const QUrl & url) { setMedia(url, startPos, max_duration); }

    void closeMedia() {
        stop();
        emit statusChanged(CloseMedia);
    }

    virtual QHash<QString, QVariant> deviceList() = 0;
    virtual QVariant currDevice() = 0;
    virtual bool setDevice(const QVariant & device) = 0;

    inline bool isInitiating() { return pstate == InitState; }
    inline bool isPlayed() { return pstate == PlayingState; }
    inline bool isPaused() { return pstate == PausedState; }
    inline bool isStopped() { return pstate == StoppedState; }

    inline PlayerState state() const { return pstate; }

    virtual qint64 position() const = 0;
    int duration() const { return max_duration; }
    inline int volume() const { return muted ? 0 : volumeVal; }
    inline int pan() const { return panVal; }
    inline int fileSize() const { return size; }

    inline virtual void openTimeOut(float /*secLimit*/) { /*stub*/ }
    inline virtual void proxy(const QString & /*proxyStr*/ = QString()) { /*stub*/ }

    inline void prebufferingLevel(float level = 1) {
        emit prebufferingChanged(prebuffering_level = level);
    }
    inline float prebufferingLevel() const { return prebuffering_level; }

    virtual bool fileInfo(const QUrl & /*uri*/, IMediaInfo * /*info*/) { return false; }
    virtual float bpmCalc(const QUrl & /*uri*/) { return 0; }
signals:
    void stateChanged(const PlayerState &);
    void statusChanged(const PlayerStatus &);

    void panChanged(int);
    void volumeChanged(int);
//    void position64Changed(qint64);
    void positionChanged(int);
//    void duration64Changed(qint64);
    void durationChanged(int);

    void prebufferingChanged(float level); // 0 .. 1
    void playbackEnding();

protected slots:
    void endOfPlayback() {
        setPosition(startPos);

        if (!looped) {
            pause();
            emit statusChanged(EndPlaingMedia);
        }
    }
public slots:
    void play(bool paused = false);
    void pause();
    void playPause() { isPlayed() ? pause() : play(); }
    void stop();

    void slidePosForward();
    void slidePosBackward();
    void slideVolForward();
    void slideVolBackward();

    void setPosition(int newPos) { setPosition(qint64(newPos)); }
    void setPosition(qint64 newPos);
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
protected slots:
    void recalcPosition() {
        int new_pos = recalcCurrentPosProcessing();
        updatePosition(new_pos);

        // cue tweak
        if (new_pos >= duration() + startPos)
            endOfPlayback();

        if (prebuffering_level < 1)
            prebufferingLevel(prebufferingLevelCalc());
    }
};

#endif // IPLAYER
