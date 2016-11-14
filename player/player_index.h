#ifndef PLAYER_INDEX
#define PLAYER_INDEX

#include <qlist.h>

#include "player_callback.h"
#include "bass_player.h"

#include "settings.h"

enum PlayerType {
    none = 0,
    bass_player = 1
};

enum PlayerInitState {
    played,
    paused,
    initiated
};

class PlayerFactory : public Core::Singleton<PlayerFactory> {
    QList<PlayerCallback> callbacks;

    friend class Core::Singleton<PlayerFactory>;
    inline PlayerFactory() : player(0) {}
    inline ~PlayerFactory() {}

    IPlayer * player;
public:
    inline IPlayer * currPlayer() { return player; }

    void registerCallback(CallbackTurn turn, QObject * obj, const char * signal, const char * slot) {
        PlayerCallback pl(turn, obj, signal, slot);
        callbacks << pl;
        if (player) pl.use(player);
    }

    IPlayer * build(QWidget * anchor, const PlayerType & newPlayerType) {
        IPlayer * old_player = player;

        switch(newPlayerType) {
    //        case bass_player: break;
            default: player = new BassPlayer(anchor);
        }

        for(QList<PlayerCallback>::Iterator cl = callbacks.begin(); cl != callbacks.end(); cl++)
            (*cl).use(player);

        player -> spectrumFreq(Settings::obj().spectrumFreqRate());
        //TODO: connect other settings

        if (old_player) {
            player -> setVolume(old_player -> volume());
            player -> setPan(old_player -> pan());
            player -> activateEQ(old_player -> eqInUse());
            player -> eqGains(old_player -> eqGains());
            player -> setMedia(old_player -> mediaUrl(), old_player -> title(), old_player -> startPosition(), old_player -> duration(), old_player -> position());

            delete old_player;
        }

        return player;
    }
};

#endif // PLAYER_INDEX
