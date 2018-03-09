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
        QMediaService * svc = player -> service();
        return svc != nullptr ?
            reinterpret_cast<QAudioOutputSelectorControl *>(svc -> requestControl(QAudioOutputSelectorControl_iid)) :
            nullptr;
    }

    void outputDeviceList(QAudioOutputSelectorControl * out, QHash<QString, QVariant> & res) {
        res.insert(defaultDeviceName(), QString());

        QStringList devices = out -> availableOutputs();
        foreach(QString device, devices)
            res.insert(device.split('\\').last(), device);
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

    inline DriverId uid() const { return driver_id_qt; }

    QHash<QString, QVariant> outputDeviceList() {
        QHash<QString, QVariant> res;

        QAudioOutputSelectorControl * out = outputControl();

        if (out) {
            outputDeviceList(out, res);
            player -> service() -> releaseControl(out);
        } /*else {
            res.insert(defaultDeviceName(), QString());

            QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
            foreach(QAudioDeviceInfo device, devices)
                res.insert(device.deviceName(), device.deviceName());
        }*/

        return res;
    }
    QVariant currOutputDevice() {
        QAudioOutputSelectorControl * out = outputControl();
        QVariant res = out ? out -> activeOutput() : QVariant();

        if (out)
            player -> service() -> releaseControl(out);

        return res;
    }
    bool setOutputDevice(const QString & device_name) {
        QAudioOutputSelectorControl * out = outputControl();

        if (out != nullptr) {
            QString new_output = device_name;
            if (new_output == defaultDeviceName())
                new_output = out -> defaultOutput();
            else {
                QHash<QString, QVariant> res;
                outputDeviceList(out, res);

                if (res.contains(new_output))
                    new_output = res.value(new_output).toString();
                else
                    new_output = out -> defaultOutput();
            }

            if (out -> activeOutput() != new_output)
                out -> setActiveOutput(new_output);

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
