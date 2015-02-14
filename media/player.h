#ifndef PLAYER_H
#define PLAYER_H

#include <QAction>
#include <QModelIndex>

#include "audio_player.h"
#include "modules/controls/clickable_label.h"
#include "modules/controls/metric_slider.h"
#include "modules/data_struct/model_item_parts/item_fields.h"

using namespace Playo3;

class Player : public AudioPlayer {
    Q_OBJECT
public:
    enum Reason {
        init,
        endMedia,
        error
    };

    static Player * instance(QObject * parent = 0);
    void playIndex(QModelIndex item, bool paused = false);
    void setStartPosition(int position);

    void setPlayButton(QAction * playAction);
    void setPauseButton(QAction * pauseAction);
    void setStopButton(QAction * stopAction);
    void setLikeButton(QAction * likeAction);
    void setMuteButton(QAction * likeAction);

    void setVolumeTrackBar(QSlider * trackBar);
    void setTrackBar(QSlider * trackBar);
    void setTimePanel(ClickableLabel * timePanel);

    inline QModelIndex playedIndex() { return currentIndex; }

    static void close() {
        delete self;
    }

signals:
    void nextItemNeeded(Player::Reason);
    void itemNotSupported(QModelIndex &);
    void itemExecError(QModelIndex &);

public slots:
    void playPause();

private slots:
    void start();
    void like();
    void mute();

    void unmuteCheck(int);
    void setVolTrackbarValue(int pos);

    void changeTrackbarValue(int);
    void setTrackbarValue(int);
    void setTrackbarMax(int);

    void invertTimeCountdown();

    void onStateChanged(MediaState newState);
    void onMediaStatusChanged(MediaStatus status);

private:
    void setItemState(int state);
    void updateItemState(bool isPlayed);
    void updateControls(bool played, bool paused, bool stopped);

    void setTimePanelVal(int millis);

    Player(QObject * parent) : AudioPlayer(parent) {
        time_forward = true;
        extended_format = true;
        prevVolumeVal = 0;

        slider = 0;
        volumeSlider = 0;
        timePanel = 0;

        muteButton = 0;
        playButton = 0;
        pauseButton = 0;
        stopButton = 0;
        likeButton = 0;


        setNotifyInterval(500);
        connect(this, SIGNAL(stateChanged(MediaState)), this, SLOT(onStateChanged(MediaState)));
        connect(this, SIGNAL(mediaStatusChanged(MediaStatus)), this, SLOT(onMediaStatusChanged(MediaStatus)));
        connect(this, SIGNAL(volumeChanged(int)), this, SLOT(unmuteCheck(int)));
    }

    static Player * self;
    Playo3::MetricSlider * slider;
    QSlider * volumeSlider;
    ClickableLabel * timePanel;

    QAction * playButton;
    QAction * pauseButton;
    QAction * stopButton;
    QAction * likeButton;
    QAction * muteButton;

    int prevVolumeVal;
    bool time_forward;
    bool extended_format;

    QModelIndex currentIndex;
};

#endif // PLAYER_H
