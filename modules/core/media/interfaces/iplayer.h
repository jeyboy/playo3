#ifndef IPLAYER
#define IPLAYER

#include <qurl.h>
#include <qdebug.h>

#include "itrackable.h"
#include "iequalizable.h"

#include "player_statuses.h"
#include "modules/core/media/interfaces/imediainfo.h"
#include "modules/core/web/utils/web_manager.h"

#define VOLUME_MULTIPLIER 10000.0
#define POSITION_MULTIPLIER 1000.0
#define PAN_MULTIPLIER 1000.0

class IPlayer : public IEqualizable, public ITrackable {
    Q_OBJECT

    PlayerState pstate;
    QTimer * itimer;
    qint64 play_pos, start_pos, size;
    int volume_val, pan_val;
    float prebuffering_level;
    bool muted, looped;   
protected:
    void updateState(const PlayerState & new_state);
    void updatePosition(const qint64 & newPos);

    virtual QString mediaUrl() const { return media_url.toString(); }

    virtual bool playProcessing(const bool & paused = false) = 0;
    void playPostprocessing();
    virtual bool resumeProcessing() = 0;
    virtual bool pauseProcessing() = 0;
    virtual bool stopProcessing() = 0;

    virtual bool newPosProcessing(const qint64 & new_pos) = 0;
    virtual bool newVolumeProcessing(const int & new_vol) = 0;
    virtual bool newPanProcessing(const int & new_pan) = 0;
    virtual float prebufferingLevelCalc() = 0;
    virtual qint64 calcFileSize() = 0;

    void initFileSize() {
        size = calcFileSize();
        qDebug() << "FILESIZE" << size;
    }

    inline void setDuration(const qint64 & new_duration) {
        ITrackable::setMaxProgress(new_duration);
        emit durationChanged((max_duration = new_duration));
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

    inline void setMedia(const QUrl & url, const qint64 & new_start_mili = 0, const qint64 & new_max_duration = 0, const qint64 & play_start_mili = 0) {
        media_url = url;
        start_pos = new_start_mili;
        play_pos = play_start_mili;
        setDuration(new_max_duration);
        updatePosition(play_pos); // update ui pos // real position changing proceed in post proc method
        updateState(url.isEmpty() ? UnknowState : InitState);
    }

    inline void updateMedia(const QUrl & url) { setMedia(url, start_pos, max_duration, play_pos); }
    inline void updateMedia(const qint64 & new_start_mili, const qint64 & new_max_duration) {
        start_pos = new_start_mili;
        play_pos = 0;
        setDuration(new_max_duration);
        setPosition(play_pos);
    }

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

    qint64 startPosition() const { return start_pos; }
    virtual qint64 position() const = 0;
    qint64 duration() const { return max_duration; }
    inline int volume() const { return muted ? 0 : volume_val; }
    inline int pan() const { return pan_val; }
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

    void muteChanged(bool);
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
        setPosition(0);

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
        newVolumeProcessing(volume_val = newVol);
        emit volumeChanged(newVol);
        emit muteChanged((muted = newVol == 0));
    }
    inline void setPan(int newPan) {
        newPanProcessing(pan_val = newPan);
        emit panChanged(newPan);
    }
protected slots:
    void recalcPosition() {
        int new_pos = position() + 250;
        updatePosition(new_pos);

        // cue tweak
        if (new_pos >= duration())
            endOfPlayback();

        if (prebuffering_level < 1)
            prebufferingLevel(prebufferingLevelCalc());
    }
};

#endif // IPLAYER
