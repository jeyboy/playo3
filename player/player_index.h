#ifndef PLAYER_INDEX
#define PLAYER_INDEX

#include <qlist.h>

#include "player_callback.h"
#include "bass_player.h"
#include "qt_player.h"

#include "settings.h"

enum PlayerType {
    none = 0,
    bass_player = 1,
    qt_player
};

enum PlayerInitState {
    played,
    paused,
    initiated
};

class PlayerFactory : public Core::Singleton<PlayerFactory> {
    QList<PlayerCallback> callbacks;

    friend class Core::Singleton<PlayerFactory>;
    inline PlayerFactory() : player(0), video_output(0) {}
    inline ~PlayerFactory() {}

    IPlayer * player;
    QWidget * video_output;
public:
    inline IPlayer * currPlayer() { return player; }

    void registerVideoOutput(QWidget * new_video_output) {
        video_output = new_video_output;
        if (player && player -> isSupportVideo())
            player -> setVideoOutput(video_output);
    }

    void registerCallback(const CallbackTurn & turn, QObject * obj, const char * signal, const char * slot) {
        PlayerCallback pl(turn, obj, signal, slot);
        callbacks << pl;
        if (player) pl.use(player);
    }

    IPlayer * build(QWidget * anchor, const PlayerType & newPlayerType) {
        IPlayer * old_player = player;

        switch(newPlayerType) {
            case qt_player: player = new QtPlayer(anchor); break;
            default: player = new BassPlayer(anchor);
        }

        for(QList<PlayerCallback>::Iterator cl = callbacks.begin(); cl != callbacks.end(); cl++)
            (*cl).use(player);

        player -> setVideoOutput(video_output);
        player -> setSpectrumFreq(Settings::obj().spectrumFreqRate());
        player -> setOutputDevice(Settings::obj().outputDevice());

        //TODO: connect other settings

        if (old_player) {
            player -> setVolume(old_player -> volume());
            player -> setPan(old_player -> pan());
            player -> activateEQ(old_player -> eqInUse());
            player -> eqGains(old_player -> eqGains(), true);
            player -> setMedia(old_player -> mediaUrl(), old_player -> title(), old_player -> startPosition(), old_player -> duration(), old_player -> position());

            delete old_player;
        }

        return player;
    }
};

#endif // PLAYER_INDEX
