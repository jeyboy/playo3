#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <qapplication.h>
#include <qurl.h>
#include <qdir.h>
#include <qvector.h>

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

    QList<QVector<int> > & getDefaultSpectrum();
    int getCalcSpectrumBandsCount();
    int getPosition() const;
    int getDuration() const;
    int getVolume() const;
    inline int getChannelsCount() const { return channelsCount; }
    float getSize() const;
    void finishRemoteFileDownloading();
    float getRemoteFileDownloadPosition();
    float getBpmValue(QUrl uri);

    int getNotifyInterval();
    void setNotifyInterval(signed int milis);

    void setMedia(QUrl mediaPath);
    void setSpectrumBandsCount(int bandsCount);
    inline int spectrumBandsCount() const { return _spectrumBandsCount; }
    void setSpectrumHeight(int newHeight);
    void setSpectrumFreq(int millis);

    MediaState state() const;

    bool isPlayed() const;
    bool isPaused() const;
    bool isStoped() const;

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
    void stoped();
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
    void setPosition(int position);

    void slideVolForward();
    void slideVolBackward();
    void setChannelVolume(int val);
    void setVolume(int val);

protected:
    int duration;

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
