#ifndef QT_PLAYER_H
#define QT_PLAYER_H

#include "modules/core/media/interfaces/iplayer.h"

//#define BASS_POSITION_MULTIPLIER 1000.0

class QtPlayer : public IPlayer {
    Q_OBJECT

//    bool is_paused;
//    QFutureWatcher<QPair<QString, qint64> > * openChannelWatcher;
//    QPair<QString, qint64> proc_channel;

    int default_device();

    bool proceedErrorState();
    QPair<QString, qint64> openChannel(const QUrl & url,  QPair<QString, qint64> & channel_params);
    void closeChannel();

    void playPreproccessing();
protected slots:
    void afterSourceOpening();

protected:
    bool deviceable() const { return false; }
    bool spectrumable() { return false; }
    bool equalizable() { return false; }
    bool isSupportTempoChanging() { return true; }

    bool playProcessing(const bool & paused = false);
    bool resumeProcessing();
    bool pauseProcessing();
    bool stopProcessing();

    bool newTempoProcessing(const int & new_tempo);
    bool newPosProcessing(const qint64 & new_pos);
    bool newVolumeProcessing(const int & new_vol);
    bool newPanProcessing(const int & new_pan);

    float prebufferingLevelCalc() { return 1; } // stub
    qint64 calcFileSize();

//    bool registerEQ();
//    bool unregisterEQ();
//    bool processEqSetGain(const int & band, const float & gain);

//    bool calcSpectrum(QVector<float> & result);
//    bool calcSpectrum(QList<QVector<float> > & result);
public:
    explicit QtPlayer(QWidget * parent);
    ~QtPlayer();

//    QHash<QString, QVariant> deviceList();
//    QVariant currDevice();
//    bool setDevice(const QVariant & device);

    qint64 position() const;

//    bool fileInfo(const QUrl & uri, IMediaInfo * info);
//    float bpmCalc(const QUrl & uri);

//    inline bool isTryingToOpenMedia() { return openChannelWatcher != 0 && openChannelWatcher -> isRunning(); }
//    inline void openTimeOut(const float & sec_limit) { BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, qMax(1000, (int)(sec_limit * 1000))); }
//    inline void proxy(const QString & proxy_str = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxy_str.isEmpty() ? NULL : QSTRING_TO_STR(proxy_str)); }
////    inline void readTimeOut(float secLimit) { BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, qMax(1000, (int)(secLimit * 1000))); }
//    inline void userAgent(const QString & user_agent = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, user_agent.isEmpty() ? NULL : QSTRING_TO_STR(user_agent)); }
};

#endif // PLAYER


#endif // QT_PLAYER_H
