#ifndef AUDIO_PLAYER_PANEL
#define AUDIO_PLAYER_PANEL

#include "bass.h"

#include "media/format.h"
#include "media/duration.h"

#include <qurl.h>

#define SLIDE_VOLUME_OFFSET 1000
#define VOLUME_MULTIPLIER 10000.0
#define POSITION_MULTIPLIER 1000.0
#define SLIDE_DURATION_PERCENT 10

class AudioPlayerPanel {
public:
    virtual ~AudioPlayerPanel() {}

    inline int getPosition() const { return BASS_ChannelBytes2Seconds(chId(), BASS_ChannelGetPosition(chId(), BASS_POS_BYTE)) * POSITION_MULTIPLIER; }
    inline int getFilePosition() const { return BASS_ChannelGetPosition(chId(), BASS_POS_BYTE); } // need to check
    inline int getDuration() const { return duration; }
    inline int getVolume() const { return volumeVal * VOLUME_MULTIPLIER; }
    inline int getChannelsCount() const { return channelsCount; }

    inline float getSize() const { return size; }
    float getRemoteFileDownloadPosition();
    float getBpmValue(QUrl uri);
    int getBitrate() const;

    inline int getNotifyInterval() { return notifyInterval; }
    void setNotifyInterval(signed int milis);

    inline void finishRemoteFileDownloading() { prevDownloadPos = 1; }

signals:
    void volumeChanged(int);
    void channelsCountChanged();

    void positionChanged(int);
    void durationChanged(int);
//    void volumeChanged(int);

public slots:
    void slidePosForward();
    void slidePosBackward();
    inline bool setPosition(int position) { return BASS_ChannelSetPosition(chId(), BASS_ChannelSeconds2Bytes(chId(), position / POSITION_MULTIPLIER), BASS_POS_BYTE); }

    void slideVolForward();
    void slideVolBackward();
    void setChannelVolume(int val);
    void setVolume(int val);

private slots:
    void signalUpdate() { emit positionChanged(BASS_ChannelBytes2Seconds(chId(), BASS_ChannelGetPosition(chId(), BASS_POS_BYTE)) * POSITION_MULTIPLIER); }

protected:
    AudioPlayerPanel() : notifyInterval(100), duration(-1), volumeVal(1.0), channelsCount(2), prevChannelsCount(0) {

    }

    virtual unsigned long chId() const = 0;

    inline void initDuration() {
        if (duration == -1)
            duration = round(BASS_ChannelBytes2Seconds(chId(), BASS_ChannelGetLength(chId(), BASS_POS_BYTE))) * POSITION_MULTIPLIER;
        durationChanged(duration);
    }

    inline void setStartPosition() {
        if (startPos == 0) return;
        if (setPosition(startPos))
            emit positionChanged(startPos);
    }

    int notifyInterval;
    int duration;
    uint startPos;

    float size;
    float prevDownloadPos;

    QUrl mediaUri;

    float volumeVal;

    int channelsCount, prevChannelsCount;
};

#endif // AUDIO_PLAYER_PANEL
