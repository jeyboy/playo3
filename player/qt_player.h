#ifndef QT_PLAYER_H
#define QT_PLAYER_H

#include "modules/core/media/interfaces/iplayer.h"

#include <qmediaplayer.h>
#include <qmediaservice.h>
#include <qaudiodeviceinfo.h>
#include <qaudiooutputselectorcontrol.h>

#define QT_VOLUME_MULTIPLIER 10.0

//player -> isAvailable()

class QtPlayer : public IPlayer {
    Q_OBJECT

    bool is_paused;
    QMediaPlayer * player;

    QAudioOutputSelectorControl * outputControl() {
        QMediaService * svc = player -> service(); \
        return svc != nullptr ?
            reinterpret_cast<QAudioOutputSelectorControl *>(svc -> requestControl(QAudioOutputSelectorControl_iid)) :
            nullptr;
    }
protected:
    bool proceedErrorState();

    bool isSupportOutputDeviceChange() const { return false; } // true
    bool isSupportTempoChanging() { return true; }
    bool isSupportSpectrumCalc() { return false; }
    bool isSupportEqualizer() { return false; }

    bool seekingBlocked() { return player -> isSeekable(); }

    bool hasAudio() const { return player -> isAudioAvailable(); }
    bool hasVideo() const { return player -> isVideoAvailable(); }

    bool isSupportVideo() { return true; }
    void setVideoOutput(QObject * output) {
        QVideoWidget * vw = reinterpret_cast<QVideoWidget *>(output);
        if (vw)
            player -> setVideoOutput(vw);
        else {
            QGraphicsVideoItem * vw = reinterpret_cast<QGraphicsVideoItem *>(output);
            if (vw) player -> setVideoOutput(vw);
        }
    }

    bool playProcessing(const bool & paused = false);
    bool resumeProcessing();
    bool pauseProcessing();
    bool stopProcessing();

    bool newTempoProcessing(const int & new_tempo);
    bool newPosProcessing(const qint64 & new_pos);
    bool newVolumeProcessing(const int & new_vol);

    float downloadingLevelCalc() { return player -> bufferStatus() / 100.0 /*1*/; } // stub
    qint64 calcFileSize();
    quint64 calcDuration() { return player -> duration(); }

//    bool registerEQ();
//    bool unregisterEQ();
//    bool processEqSetGain(const int & band, const float & gain);

//    bool calcSpectrum(QVector<float> & result);
//    bool calcSpectrum(QList<QVector<float> > & result);
private slots:
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

public:
    explicit QtPlayer(QWidget * parent);
    ~QtPlayer();

    QHash<QString, QVariant> outputDeviceList() {
        QHash<QString, QVariant> res;

        QAudioOutputSelectorControl * out = outputControl();

        if (out) {
            QStringList devices = out -> availableOutputs();
            foreach(QString device, devices)
                res.insert(device, device);

            player -> service() -> releaseControl(out);
        } else {
            QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
            foreach(QAudioDeviceInfo device, devices)
                res.insert(device.deviceName(), device.deviceName());
        }

        return res;
    }
    QVariant currOutputDevice() {
        QAudioOutputSelectorControl * out = outputControl();
        QVariant res = out ? out -> activeOutput() : QVariant();
        player -> service() -> releaseControl(out);
        return res;
    }
    bool setOutputDevice(const QVariant & device) {
        QAudioOutputSelectorControl * out = outputControl();
        if (out != nullptr) {
            out -> setActiveOutput(device.toString());
            player -> service() -> releaseControl(out);
            return true;
        }

        return false;
    }

    qint64 position() const;

    bool fileInfo(const QUrl & uri, IMediaInfo * info);

//    bool setProxy(const QString & /*proxy_str*/ = QString()) { return false; }
};

#endif // QT_PLAYER_H
