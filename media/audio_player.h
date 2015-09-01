#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <qapplication.h>
#include <qurl.h>
#include <qdir.h>
#include <qvector.h>

#include "math.h"

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"

#include "media/notify_timer.h"
#include "media/format.h"
#include "media/duration.h"

#ifdef Q_OS_WIN
    void __stdcall endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
    void __stdcall endTrackDownloading(HSYNC, DWORD, DWORD, void * user);
#else
    void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
    void endTrackDownloading(HSYNC, DWORD, DWORD, void * user);
#endif

// TODO: add proxy realization
//if (MESS(41,BM_GETCHECK,0,0))
//    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,NULL); // disable proxy
//else
//    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,proxy); // enable proxy

class AudioPlayer : public QObject {
    Q_OBJECT

    Q_ENUMS(MediaStateFlags)
    Q_ENUMS(MediaStatusFlags)

    int openChannel(QString path);
    int openRemoteChannel(QString path);
    void closeChannel();
public:
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

    explicit AudioPlayer(QObject * parent = 0);
    ~AudioPlayer();

    inline QList<QVector<int> > & getDefaultSpectrum() { return defaultSpectrum; }
    int getCalcSpectrumBandsCount();
    inline int getPosition() const { return BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)) * 1000; }
    inline int getDuration() const { return duration; }
    inline int getVolume() const { return volumeVal * 10000; }
    inline int getChannelsCount() const { return channelsCount; }
    inline float getSize() const { return size; }
    inline void finishRemoteFileDownloading() {
        prevDownloadPos = 1;
        initDuration();
    }
    float getRemoteFileDownloadPosition();
    float getBpmValue(QUrl uri);

    inline int getNotifyInterval() { return notifyInterval; }
    void setNotifyInterval(signed int milis);

    virtual inline void startProccessing() {}

    inline void setMedia(QUrl mediaPath, uint start_pos = 0, int length = -1) {
        mediaUri = mediaPath;
        currentState = InitState;
        startPos = start_pos;
        if ((duration = length) > 0) {
            initDuration();
            setStartPosition();
        }
    }
    void setSpectrumBandsCount(int bandsCount);
    inline int spectrumBandsCount() const { return _spectrumBandsCount; }
    inline void setSpectrumHeight(int newHeight) { spectrumHeight = newHeight; }
    inline void setSpectrumFreq(int millis) { spectrumTimer -> setInterval(millis); }

    inline MediaState state() const { return currentState; }

    inline bool isPlayed() const { return currentState == PlayingState; }
    inline bool isPaused() const { return currentState == PausedState; }
    inline bool isStoped() const { return currentState == StoppedState; }

    void registerEQ();
    void unregisterEQ();

    inline QMap<int, int> eqGains() const { return eqBandsGain; }
    inline void setEqGains(QMap<int, int> gains) { eqBandsGain = gains; }
    void setEQBand(int band, float gain);
    inline QMap<int, QString> bands() const { return eqBands; }

signals:
    void volumeChanged(int);
    void playbackEnded();
    void downloadEnded();
    void stateChanged(MediaState);
    void mediaStatusChanged(MediaStatus);
    void spectrumChanged(QList<QVector<int> >);
    void channelsCountChanged();

    void positionChanged(int);
    void durationChanged(int);
//    void volumeChanged(int);

private slots:
    void started();
    inline void stoped() { currentState = StoppedState; }
    void signalUpdate();
    void calcSpectrum();

public slots:
    void registerEQ(bool registrate);

    void play();
    void pause();
    void resume();
    void stop();
    void endOfPlayback();

    void slidePosForward();
    void slidePosBackward();
    inline bool setPosition(int position) { return BASS_ChannelSetPosition(chan, BASS_ChannelSeconds2Bytes(chan, position / 1000.0), BASS_POS_BYTE); }

    void slideVolForward();
    void slideVolBackward();
    void setChannelVolume(int val);
    void setVolume(int val);
protected:
    int duration;
    uint startPos;

    inline void initDuration() {
        if (duration == -1)
            duration = round(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE))) * 1000;
        durationChanged(duration);
    }

    inline void setStartPosition() {
        if (startPos == 0) return;
        if (setPosition(startPos))
            emit positionChanged(startPos);
    }

private:
    HFX _fxEQ;
    bool useEQ;
    QMap<int, QString> eqBands;
    QMap<int, int> eqBandsGain;

    float fastSqrt(float x);

    QVector<int> spectrumPoints;
    QVector<int> spectrumComplexPoints;

    QVector<int> getSpectrum();
    QList<QVector<int> > getComplexSpectrum();
    int getBitrate() const;

    QUrl mediaUri;

    float volumeVal;

    int channelsCount, prevChannelsCount;
    int notifyInterval;
    float size;
    float prevDownloadPos;

    int _spectrumBandsCount;
    int spectrumHeight;
    int defaultSpectrumLevel;
    QList<QVector<int> > defaultSpectrum;

    MediaState currentState;

    unsigned long chan;
    HSYNC syncHandle, syncDownloadHandle;
    NotifyTimer * notifyTimer;
    NotifyTimer * spectrumTimer;
};

#endif // AUDIO_PLAYER_H
