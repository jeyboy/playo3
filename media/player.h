//TODO: improve singleton and add destructor

#ifndef PLAYER_H
#define PLAYER_H

#include <QSlider>
#include <QAction>
#include <QIcon>

#include <QtCore/qmath.h>

//#include "model/model.h"
//#include "model/view.h"
#include "audio_player.h"
#include "modules/controls/clickable_label.h"

using namespace Playo3;

class Player : public AudioPlayer {
    Q_OBJECT
public:
    static Player * instance(QObject * parent = 0);
    void playItem(QUrl url, bool paused = false);
    void setStartPosition(int position);

//    void setActivePlaylist(View * itemPlaylist);
    void setPlayButton(QAction * playAction);
    void setPauseButton(QAction * pauseAction);
    void setStopButton(QAction * stopAction);
    void setLikeButton(QAction * likeAction);
    void setMuteButton(QAction * likeAction);

    void setVolumeTrackBar(QSlider * trackBar);
    void setTrackBar(QSlider * trackBar);
    void setTimePanel(ClickableLabel * timePanel);

    static void close() {
        delete self;
    }

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
    QSlider * slider;
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
};

#endif // PLAYER_H
