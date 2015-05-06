#ifndef PLAYER_H
#define PLAYER_H

#include <qaction.h>
#include <QModelIndex>

#ifdef Q_OS_WIN
    #include <QWinTaskbarButton>
    #include <QWinTaskbarProgress>
#endif

#include "mediainfo.h"
#include "audio_player.h"
#include "modules/controls/clickable_label.h"
#include "modules/controls/metric_slider.h"
#include "modules/data_struct/model_interface.h"

using namespace Playo3;

class MediaInfo;

namespace Playo3 {
    class IModel;
}

class Player : public AudioPlayer {
    Q_OBJECT
public:
    enum Reason {
        init,
        endMedia,
        noMedia,
        stalled,
        error,
        refreshNeed
    };

    static Player * instance(QWidget * parent = 0);
    inline static void close() { delete self; }

    void eject(bool updateState = true);
    bool playIndex(QModelIndex item, bool paused = false, uint start = 0);
    void setStartPosition(int position);

    void setPlayButton(QAction * playAction);
    void setPauseButton(QAction * pauseAction);
    void setStopButton(QAction * stopAction);
    void setLikeButton(QAction * likeAction);
    void setMuteButton(QAction * likeAction);

    void setVolumeTrackBar(QSlider * trackBar);
    void setTrackBar(QSlider * trackBar);
    void setTimePanel(ClickableLabel * timePanel);

    QModelIndex playedIndex();
    inline IModel * currentPlaylist() const { return current_model; }
    inline IItem * playedItem() const { return current_item; }
    inline QString playedItemTreePath() const { return current_item -> buildTreeStr(); }

    void getFileInfo(QUrl uri, MediaInfo * info);

signals:
    void nextItemNeeded(Player::Reason);
//    void itemNotSupported(QModelIndex);
//    void itemNotExisted(QModelIndex);
//    void itemNotAccessable(QModelIndex);
//    void itemExecError(QModelIndex);

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
    Player(QWidget * parent);

    void setItemState(int state);
    void updateItemState(bool isPlayed);
    void updateControls(bool played, bool paused, bool stopped);

    void setTimePanelVal(int millis);

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

    IModel * current_model;
    IItem * current_item;

    #ifdef Q_OS_WIN
        QWinTaskbarButton * stateButton;
        QWinTaskbarProgress * stateProgress;
    #endif
};

#endif // PLAYER_H
