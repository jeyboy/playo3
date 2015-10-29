#ifndef PLAYER_INDEX
#define PLAYER_INDEX

#include <qlist.h>

#include "player_callback.h"
#include "bass_player.h"

enum PlayerType {
    none = 0,
    bass_player = 1
};

class PlayerFactory : public Core::Singleton<PlayerFactory> {
    QList<PlayerCallback> callbacks;

    friend class Core::Singleton<PlayerFactory>;
    inline PlayerFactory() : player(0) {}

    IPlayer * player;
public:
    inline IPlayer * currPlayer() { return player; }

    void registerCallback(CallbackTurn turn, QObject * obj, const char * signal, const char * slot) {
        PlayerCallback pl(turn, obj, signal, slot);
        callbacks << pl;
        if (player) pl.use(player);
    }

    IPlayer * build(QWidget * anchor, const PlayerType & newPlayerType) {
        delete player;

        switch(newPlayerType) {
    //        case bass_player: break;
            default: player = new BassPlayer(anchor);
        }

        for(QList<PlayerCallback>::Iterator cl = callbacks.begin(); cl != callbacks.end(); cl++)
            (*cl).use(player);

        return player;
    }
};

#endif // PLAYER_INDEX
