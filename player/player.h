#ifndef PLAYER_H
#define PLAYER_H

#include <qaction.h>

#ifdef Q_OS_WIN
    #include <qwintaskbarbutton.h>
    #include <qwintaskbarprogress.h>
#endif

#include "audio_player.h"
#include "modules/controls/clickable_label.h"
#include "modules/controls/metric_slider.h"
#include "modules/models/model_interface.h"
#include "modules/core/media/mediainfo.h"
#include "modules/core/interfaces/singleton.h"

using namespace AudioPlayer;
///////////// remove later
namespace Models { class IModel; }
using namespace Models;
using namespace Core;

namespace Core { namespace Media { class MediaInfo; } }
///////////////////////////////

class Player : public AudioPlayer::Base, public Singleton<Player> {
    Q_OBJECT
public:
    enum Reason { init, endMedia, noMedia, stalled, error, refreshNeed };

    void setContainer(QWidget * parent) {
        #ifdef Q_OS_WIN
            stateButton = new QWinTaskbarButton(parent);
            parent -> winId(); // generate native object for windowHandle()
            stateButton -> setWindow(parent -> windowHandle());
            stateProgress = stateButton -> progress();
            stateProgress -> setMinimum(0);
        #endif
    }

    void eject(bool updateState = true);
    void playIndex(const QModelIndex & item, bool paused = false, uint start = 0, int duration = -1);

    void setPlayButton(QAction * playAction);
    void setPauseButton(QAction * pauseAction);
    void setStopButton(QAction * stopAction);
    void setCyclingButton(QAction * cycleAction);

    void setLikeButton(QAction * likeAction);
    void setMuteButton(QAction * likeAction);

    void setPanTrackBar(QSlider * trackBar);
    void setVolumeTrackBar(QSlider * trackBar);
    void setTrackBar(QSlider * trackBar);
    void setTimePanel(Controls::ClickableLabel * timePanel);

    QModelIndex playedIndex();
    inline IModel * currentPlaylist() const { return current_model; }
    inline IItem * playedItem() { return current_item; }
    inline QString playedItemTreePath() const { return current_item -> buildTreeStr(); }

    bool getFileInfo(QUrl uri, MediaInfo * info);
    void playedIndexIsNotExist();
    void playedIndexIsInvalid();

signals:
    void nextItemNeeded(Player::Reason);

public slots:
    void playPause();

private slots:
    void start();
    void like();
    void mute();

    void unmuteCheck(int);
    void setVolTrackbarValue(int pos);
    void setPanTrackbarValue(int pos);

    void changeTrackbarValue(int);
    void setTrackbarValue(int);
    void setTrackbarMax(int);

    void invertTimeCountdown();

    void onStateChanged(MediaState newState);
    void onMediaStatusChanged(MediaStatus status);
protected:
    inline void startProccessing() { setItemState(ItemState::proccessing); }
    inline void endProccessing() { setItemState(-(ItemState::proccessing | ItemState::not_exist | ItemState::not_supported)); }
private:
    friend class Singleton<Player>;
    Player();

    void setStartPosition();
    void setItemState(int state);
    void updateItemState(bool isPlayed);
    void updateControls(bool played, bool paused, bool stopped);

    void setTimePanelVal(int millis);

    Controls::MetricSlider * slider;
    QSlider * volumeSlider, * panSlider;
    Controls::ClickableLabel * timePanel;

    QAction * playButton;
    QAction * pauseButton;
    QAction * stopButton;
    QAction * cycleButton;

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
