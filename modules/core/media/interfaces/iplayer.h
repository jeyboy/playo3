#ifndef IPLAYER
#define IPLAYER

#include <qurl.h>
#include <qdebug.h>

#include "itrackable.h"
#include "iequalizable.h"
#include "ideviceable.h"

#include "player_statuses.h"
#include "modules/core/media/interfaces/imediainfo.h"
#include "modules/core/web/utils/web_manager.h"

class IPlayer : public IEqualizable, public ITrackable, public IDeviceable {
    Q_OBJECT

    PlayerState pstate;
    QTimer * itimer;
    qint64 play_pos, start_pos, size;
    int volume_val, pan_val, tempo_val;
    float prebuffering_level;
    bool muted, looped;   
protected:
    void updateState(const PlayerState & new_state);
    void updatePosition(const qint64 & newPos);

    virtual bool playProcessing(const bool & paused = false) = 0;
    void playPostprocessing();
    virtual bool resumeProcessing() = 0;
    virtual bool pauseProcessing() = 0;
    virtual bool stopProcessing() = 0;

    virtual bool newTempoProcessing(const int & /*new_tempo*/) { return false; }
    virtual bool newPosProcessing(const qint64 & new_pos) = 0;
    virtual bool newVolumeProcessing(const int & new_vol) = 0;
    virtual bool newPanProcessing(const int & new_pan) = 0;
    virtual float prebufferingLevelCalc() { return 1; }
    virtual qint64 calcFileSize() { return -1; }

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
    QString media_title;
    qint64 max_pos, max_duration;

public:
    explicit IPlayer(QWidget * parent);
    virtual ~IPlayer() {}

    //INFO: position calcs in millis by default

    static int volumeMin() { return 0; }
    static int volumeMax() { return 1000; }
    static int volumeDefault() { return 1000; }

    static int panMin() { return -1000; }
    static int panMax() { return 1000; }
    static int panDefault() { return 0; }

    static int tempoMin() { return -1000; }
    static int tempoMax() { return 1000; }
    static int tempoDefault() { return 0; }

    QUrl mediaUrl() const { return media_url; }
    QString title() const { return media_title; }

    inline void setMedia(const QUrl & url, const QString & name = QString(), const qint64 & new_start_mili = 0, const qint64 & new_max_duration = 0, const qint64 & play_start_mili = 0) {
        media_title = name;
        media_url = url;
        start_pos = new_start_mili;
        play_pos = play_start_mili;
        setDuration(new_max_duration);
        updatePosition(play_pos); // update ui pos // real position changing proceed in post proc method
        updateState(url.isEmpty() ? UnknowState : InitState);
    }

    inline void updateMedia(const QUrl & url, const QString & name) { setMedia(url, name, start_pos, max_duration, play_pos); }
    inline void updateMedia(const qint64 & new_start_mili, const qint64 & new_max_duration) {
        start_pos = new_start_mili;
        play_pos = 0;
        setDuration(new_max_duration);
        setPosition(play_pos);
    }

    void closeMedia() {
        stop();
        emit statusChanged(media_title, CloseMedia);
    }

    virtual QHash<QString, QVariant> deviceList() = 0;
    virtual QVariant currDevice() = 0;
    virtual bool setDevice(const QVariant & device) = 0;

    inline bool isInitiating() { return pstate == InitState; }
    inline bool isPlayed() { return pstate == PlayingState; }
    inline bool isPaused() { return pstate == PausedState; }
    inline bool isStopped() { return pstate == StoppedState; }

    virtual bool isSupportVideo() { return false; }
    virtual bool isSupportTempoChanging() { return false; }

    inline PlayerState state() const { return pstate; }

    qint64 startPosition() const { return start_pos; }
    virtual qint64 position() const = 0;
    qint64 duration() const { return max_duration; }
    inline int volume() const { return muted ? 0 : volume_val; }
    inline int pan() const { return pan_val; }
    inline int tempo() const { return tempo_val; }
    inline int fileSize() const { return size; }

    inline virtual void openTimeOut(float /*secLimit*/) { /*stub*/ }
    inline virtual void proxy(const QString & /*proxyStr*/ = QString()) { /*stub*/ }

    inline void prebufferingLevel(const float & level = 1) {
        emit prebufferingChanged(prebuffering_level = level);
    }
    inline float prebufferingLevel() const { return prebuffering_level; }

    virtual bool fileInfo(const QUrl & /*uri*/, IMediaInfo * /*info*/) { return false; }
    virtual float bpmCalc(const QUrl & /*uri*/) { return 0; }
signals:
    void stateChanged(const PlayerState &);
    void statusChanged(const QString &, const PlayerStatus &);

    void muteChanged(bool);
    void panChanged(int);
    void volumeChanged(int);
    void tempoChanged(int);
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
            emit statusChanged(media_title, EndPlaingMedia);
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

    void setTempo(const int & new_tempo) {
        if (!isSupportTempoChanging()) return;

        tempo_val = new_tempo;

        tempo_val = qMax(tempo_val, tempoMin());
        tempo_val = qMin(tempo_val, tempoMax());

        newTempoProcessing(tempo_val);
        emit tempoChanged(tempo_val);
    }

    void setPosition(const int & new_pos) { setPosition(qint64(new_pos)); }
    void setPosition(const qint64 & new_pos);
    void mute(const bool & enable = false);
    void loop(const bool & enable = false) { looped = enable; }
    inline void setVolume(const int & new_vol) {
        volume_val = new_vol;

        volume_val = qMax(volume_val, volumeMin());
        volume_val = qMin(volume_val, volumeMax());

        newVolumeProcessing(volume_val);
        emit volumeChanged(volume_val);
        emit muteChanged((muted = volume_val == 0));
    }
    inline void setPan(const int & new_pan) {
        pan_val = new_pan;

        pan_val = qMax(pan_val, panMin());
        pan_val = qMin(pan_val, panMax());

        newPanProcessing(pan_val);
        emit panChanged(pan_val);
    }
protected slots:
    void recalcPosition() {
        int new_pos = position() + 250; // INFO: tweak for compensation of float disbalance
        updatePosition(new_pos);

        if (new_pos >= duration()) //INFO: cue tweak
            endOfPlayback();

        if (prebuffering_level < 1)
            prebufferingLevel(prebufferingLevelCalc());
    }
};

#endif // IPLAYER
